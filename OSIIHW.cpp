#include <iostream>
#include <cstdio>
#include <string>
#include <list>
#include <algorithm>

#define abs(X) ((X) < 0 ? ((X)*-1) : (X))

using namespace std;

class request{
	public:
		request();
		request(double time, int track_num);
		string str();
		double time;
		int track_num;
		bool operator==(const request& b);
		bool operator!=(const request& b);
};

typedef list<request> dss;
typedef list<request>::iterator lrit;
#define NVR request(-1, -1)

class disk{
	public:
		disk(int cylinders, int init_set, int direct);
		void set_schedule_algo(string sch);
		void add_request(int track_num);
		void add_request(double time, int track_num);
		request schedule();
		
		void show_all_request();
		string get_algo();
		int get_cylinders();
		int get_current_headseat();
		int get_current_direct();
		int get_total_cylinders();
		
	private:
		request fcfs();
		request sstf();
		request scan();
		request look();
		request cscan();
		request clook();
		
		string schedule_name;
		int cylinders, currcyld, direct, total_cylinders;
		request (disk::*schedule_algo)();
		list<request> request_set;
};

int main(int argc, char* argv[]){
	
	char buffer[20];
	string schedule_algo = "";
	int cylinders = 0, init = 0, direct = 0;
	FILE* inputfile = fopen("basic.txt", "r");
	fscanf(inputfile, "%s", buffer);
	fscanf(inputfile, "%d %d %d", &cylinders, &init, &direct);
	disk localdisk_D(cylinders, init, direct);
	localdisk_D.set_schedule_algo(buffer);
	
	for(int tmp = 0; !feof(inputfile);){
		fscanf(inputfile, "%d,", &tmp);
		if(tmp == -1) break;
		//cout << tmp << '\n';
		localdisk_D.add_request(tmp);
	}
	fclose(inputfile);
	
	request tmp;
	cout << "DISK SCHEDULE START WITH INIT AS : " << '\n' << '\n';
	cout << "Algorithm : " << localdisk_D.get_algo() << '\n';
	cout << "Total cylinders : " << localdisk_D.get_cylinders() << '\n';
	cout << "Init head seat : " << localdisk_D.get_current_headseat() << '\n';
	cout << "Init direct : " << localdisk_D.get_current_direct() << '\n';
	cout << "Disk requests : " << endl;
	localdisk_D.show_all_request();
	cout << '\n' << "Head track : " << '\n'; 
	while((tmp = localdisk_D.schedule()) != NVR) cout << tmp.str() << endl;
	cout << "\ntotal moved cylinders : " << localdisk_D.get_total_cylinders() << endl;
	
	return 0;
}

request::request() : time(0.0), track_num(0){}
request::request(double time, int track_num) : time(time), track_num(track_num){}
string request::str(){ string fmtime = to_string(time);
	return string('(' + fmtime.substr(0, fmtime.find(".", 0) + 2) + ',' + to_string(track_num) + ')'); }
bool request::operator==(const request& b){ return (time == b.time && track_num == b.track_num); }
bool request::operator!=(const request& b){ return (time != b.time || track_num != b.track_num); } 

disk::disk(int cylinders, int init_set, int direct) : schedule_name("FCFS"), schedule_algo(&disk::fcfs), cylinders(cylinders), currcyld(init_set), direct(direct), total_cylinders(0){}
void disk::set_schedule_algo(string sch){
	transform(sch.begin(), sch.end(), sch.begin(), ::toupper);
	schedule_name = sch;
	try{
		if(schedule_name == "FCFS") schedule_algo = &disk::fcfs;
		else if(schedule_name == "SSTF") schedule_algo = &disk::sstf;
		else if(schedule_name == "SCAN") schedule_algo = &disk::scan;
		else if(schedule_name == "LOOK") schedule_algo = &disk::look;
		else if(schedule_name == "CSCAN") schedule_algo = &disk::cscan;
		else if(schedule_name == "CLOOK") schedule_algo = &disk::clook;
		else throw "Not a valid disk schedule algorithm.";
	}
	catch(const char* err){
		cerr << err << endl;
	}
	return;
}
void disk::add_request(int track_num){ disk::add_request(0.0, track_num); }
void disk::add_request(double time, int track_num){
	request pushin(time, track_num);
	request_set.push_back(pushin);
	return;
}
request disk::schedule(){
	request next =  (this->*schedule_algo)();
	if(next != NVR) total_cylinders += abs(next.track_num - currcyld);
	currcyld = next.track_num;
	return next;
}
void disk::show_all_request(){
	list<request>::iterator it = this->request_set.begin();
	for(; it != this->request_set.end(); it++) cout << '(' << it->time << ',' << it->track_num << ')' << endl;
	return;
}
string disk::get_algo(){ return schedule_name; }
int disk::get_cylinders(){ return cylinders; }
int disk::get_current_headseat(){ return currcyld; }
int disk::get_current_direct(){ return direct; }
int disk::get_total_cylinders(){ return total_cylinders; }

request disk::fcfs(){
	if(request_set.empty()) return NVR;
	list<request>::iterator first = request_set.begin();
	for(lrit curr = first; curr != request_set.end(); curr++) if(curr->time < first->time) first = curr;
	request ret = *first;
	request_set.erase(first);
	return ret;
}
request disk::sstf(){
	if(request_set.empty()) return NVR;
	list<request>::iterator closest = request_set.begin();
	for(lrit curr = closest; curr != request_set.end(); curr++){
		if(abs(curr->track_num - currcyld) < abs(closest->track_num - currcyld))
			closest = curr;
	}
	request ret = *closest;
	request_set.erase(closest);
	return ret;
}
request disk::scan(){
	if(request_set.empty()) return NVR;
	
	if(currcyld == cylinders - 1) direct = 1;
	else if(currcyld == 0) direct = 0;
	
	list<request>::iterator next = request_set.end();
	request ret;
	if(direct == 0){
		for(lrit it = request_set.begin(); it != request_set.end(); it++){
			if(next == request_set.end() && it->track_num >= currcyld) next = it; 
			else if(it->track_num >= currcyld && it->track_num - currcyld < next->track_num - currcyld) next = it;
		}
		if(next == request_set.end()) ret = request(0.0, cylinders - 1);
		else ret = *next;
	}
	else{
		for(lrit it = request_set.begin(); it != request_set.end(); it++){
			if(next == request_set.end() && it->track_num <= currcyld) next = it; 
			else if(it->track_num <= currcyld && it->track_num - currcyld > next->track_num - currcyld) next = it;
		}
		if(next == request_set.end()) ret = request(0.0, 0);
		else ret = *next;
	}
	if(next != request_set.end()) request_set.erase(next);
	return ret;
}
request disk::look(){
	if(request_set.empty()) return NVR;
	list<request>::iterator next = request_set.end();
	request ret;
	while(next == request_set.end()){
		if(direct == 0){
			for(lrit it = request_set.begin(); it != request_set.end(); it++){
				if(next == request_set.end() && it->track_num > currcyld) next = it; 
				else if(it->track_num > currcyld && it->track_num - currcyld < next->track_num - currcyld) next = it;
			}
			if(next == request_set.end()) direct = 1;
			else ret = *next;
		}
		else{
			for(lrit it = request_set.begin(); it != request_set.end(); it++){
				if(next == request_set.end() && it->track_num <= currcyld) next = it; 
				else if(it->track_num <= currcyld && it->track_num - currcyld > next->track_num - currcyld) next = it;
			}
			if(next == request_set.end()) direct = 0;
			else ret = *next;
		}
	}
	if(next != request_set.end()) request_set.erase(next);
	return ret;
}
request disk::cscan(){
	if(request_set.empty()) return NVR;
	
	if(direct == 0 && currcyld == cylinders - 1) return request(0.0, 0);
	else if(direct == 1 && currcyld == 0) return request(0.0, cylinders - 1);
	
	list<request>::iterator next = request_set.end();
	request ret;
	if(direct == 0){
		for(lrit it = request_set.begin(); it != request_set.end(); it++){
			if(next == request_set.end() && it->track_num >= currcyld) next = it; 
			else if(it->track_num >= currcyld && it->track_num - currcyld < next->track_num - currcyld) next = it;
		}
		if(next == request_set.end()) ret = request(0.0, cylinders - 1);
		else ret = *next;
	}
	else{
		for(lrit it = request_set.begin(); it != request_set.end(); it++){
			if(next == request_set.end() && it->track_num <= currcyld) next = it; 
			else if(it->track_num <= currcyld && it->track_num - currcyld > next->track_num - currcyld) next = it;
		}
		if(next == request_set.end()) ret = request(0.0, 0);
		else ret = *next;
	}
	if(next != request_set.end()) request_set.erase(next);
	return ret;
}
request disk::clook(){
	if(request_set.empty()) return NVR;
	list<request>::iterator next = request_set.end();
	request ret;
	if(direct == 0){
		for(lrit it = request_set.begin(); it != request_set.end(); it++){
			if(next == request_set.end() && it->track_num > currcyld) next = it; 
			else if(it->track_num > currcyld && it->track_num - currcyld < next->track_num - currcyld) next = it;
		}
		if(next == request_set.end()){
			for(lrit it = next = request_set.begin(); it != request_set.end(); it++){
				if(it->track_num < next->track_num) next = it;
			}
		}
	}
	else{
		for(lrit it = request_set.begin(); it != request_set.end(); it++){
			if(next == request_set.end() && it->track_num <= currcyld) next = it; 
			else if(it->track_num <= currcyld && it->track_num - currcyld > next->track_num - currcyld) next = it;
		}
		if(next == request_set.end()){
			for(lrit it = next = request_set.begin(); it != request_set.end(); it++){
				if(it->track_num > next->track_num) next = it;
			}
		}
	}
	ret = *next;
	if(next != request_set.end()) request_set.erase(next);
	return ret;
}

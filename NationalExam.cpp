#include<iostream>
#include<string>
#include<ctime>
#include<iomanip>
#include<sstream>
#include<fstream>
#include<vector>

using namespace std;

void showHelpMenu(){
	cout<<"============================================================\n"<<endl;
	cout<<setw(10)<<"HELP MENU"<<endl;
	cout<<"\n============================================================"<<endl;
	cout<<"add <location> \t \t \t \t \t: Add a new location"<<endl;
	cout<<"delete <location> \t \t \t \t: Delete an existing location"<<endl;
	cout<<"record <location><disease><cases>\t \t : Record a disease and its cases"<<endl;
	cout<<"list locations \t \t \t \t \t: List all  locations"<<endl;
	cout<<"where <location> \t \t \t \t: Finds where a disease exists"<<endl;
	cout<<"cases <location><disease> \t \t \t: Find cases of a disease in location"<<endl;
	cout<<"cases <disease> \t \t \t \t: Find total cases of a given disease"<<endl;
	cout<<"help \t \t \t \t \t \t: Prints user manual"<<endl;
	cout<<"exit \t \t \t \t \t \t: Exists the program"<<endl;	
}

string getCurrentTime(){
	time_t now = time(nullptr);
	
	char timeString[50];
	strftime(timeString, sizeof(timeString), "%a %b %d %H:%M:%S CAT %Y", localtime(&now));
	return timeString;
}
string toUpperCase(const string& str){
	string result;
	for(char c : str){
		result+= toupper(c);
	}
	return result;
}
vector<string> split(string str){
	istringstream iss(str);
	vector<string> tokens;
	string token;
	
	while(getline(iss, token, ',')){
		
		tokens.push_back(token);
		
	}
	return tokens;
}

bool addLocation(string location){
	fstream file("data.csv", ios::in);
	string line;
	bool locationExists = false;
	
	while(getline(file, line)){
		size_t pos = line.find(location);
		if(pos != string::npos){
			locationExists = true;
			break;
		}
	}
	file.close();
	
	if(!locationExists){
		fstream file("data.csv", ios::out | ios::app);
		file<<location<<",\n";
		file.close();
		
		return true;
	}else{
		return false;
	}
}
void listLocations(){
	fstream file("data.csv", ios::in);
	string line;
	
	while(getline(file, line)){
		string currentLocation;
		istringstream iss(line);
		getline(iss, currentLocation, ',');
		cout<<currentLocation<<"\n"<<endl;
	}
}

void recordDisease(string location, string disease, string cases){
	fstream file("data.csv", ios::in);
	fstream temp("temp.csv", ios::out | ios::app);
	
	bool lineFound, lineWithDisease = false;
	string line;
	string newLocation = toUpperCase(location);
	string newLine = newLocation+","+disease+","+cases;
	
	while(getline(file, line)){
		size_t pos = line.find(newLocation);
		
		if(pos != string::npos){
			
			lineFound = true;
			vector<string> splitLine= split(line);
			
			if(splitLine.size()==1){
				temp<<newLine<<endl;
				cout<<"Disease and cases recorded!"<<endl;
			}else{
				temp<<line<<endl;
				lineWithDisease= true;
//				cout<<"Disease already there!"<<endl;
			}
		}else{
			temp<<line<<endl;
		}
	
		
	}
		if(!lineFound || lineWithDisease){
			temp<<newLine<<endl;
		}
		temp.close();
		file.close();
		
		remove("data.csv");
		rename("temp.csv", "data.csv");
	
}
void listDiseases(){
	fstream file("data.csv", ios::in);
	string line;
	vector<string> splitLine;
	while(getline(file,line)){
		splitLine = split(line);
		if(splitLine.size()==3){
			cout<<splitLine[1]<<endl;
		}
//		size_t pos = line.find()
	}
}
void where(string disease){
	fstream file("data.csv", ios::in);
	string line;
	vector<string> splitLine;
	while(getline(file, line)){
		splitLine= split(line);
		if(splitLine.size()==3){
			if(splitLine[1] == disease){
				cout<<splitLine[0]<<endl;
			}
		}
	}
}
void casesInLocations(string location, string disease){
	fstream file("data.csv", ios::in);
	string line;
	vector<string>splitLine;
	while(getline(file, line)){
		splitLine = split(line);
		if(splitLine.size()==3){
			if(splitLine[1] == disease && splitLine[0]==location){
				cout<<splitLine[2]<<endl;
			}
		}
	}
}

void totalCases(string disease){
	
	fstream file("data.csv", ios::in);
	string line;
	vector<string>splitLine;
	int count=0;
	while(getline(file, line)){
		splitLine = split(line);
		if(splitLine.size()==3){
			if(splitLine[1]== disease){
				int caseNumber = stoi(splitLine[2]);
				count += caseNumber;
			}
		}
	}
	cout<<count<<endl;
	
}
int main(){
	string input;
	bool exit = false;
	cout<<"============================================================"<<endl;
	cout<<"Welcome to Diseases Cases Reporting System!"<<endl;
	cout<<"============================================================"<<endl;
	cout<<"\nThis is developed by Larissa Teta as practical "<<endl;
	cout<<"\nEvaluation for end of year 3 "<<endl;	
	cout<<"============================================================"<<endl;
	cout<<"Starting time: "<< getCurrentTime()<<endl;
	
	cout<<"Need help? Type Help' and press Enter key!"<<endl;
	cout<<endl;
	
	while(3>0){
		cout<<"Console >";
		getline(cin, input);	
		if(input.find("add ")==0){
			istringstream iss(input);
			string command, location;
			iss>>command>>location;
			
			string upperLocation = toUpperCase(location);
			
			bool result = addLocation(upperLocation);
			
			if(result){
				cout<<"Location "<<upperLocation<< " is successfully added!"<<endl;
			}else{
				cout<<"Failed to add location "<<upperLocation<<endl;
			}
		}else if(input.find("list ")==0){
			istringstream iss(input);
			string command, line;
			
			iss>>command>>line;
			if(line == "locations"){
				listLocations();
			}else if(line == "diseases"){
				listDiseases();
			}
		}else if(input.find("record ")==0){
			istringstream iss(input);
			string command, location, lowerDisease, cases;
			iss>>command>>location>>lowerDisease>>cases;
			string disease = toUpperCase(lowerDisease);
			recordDisease(location, disease, cases);
		}else if(input.find("where")==0){
			istringstream iss(input);
			string command, lowerDisease;
			iss>>command>>lowerDisease;
			
			string disease= toUpperCase(lowerDisease);
			where(disease);
			
		}else if(input.find("cases")==0){
			vector<string> request;
			istringstream iss(input);
			string command, loc, dis, token;
			while(iss>>token){
				request.push_back(token);
			}
			if(request.size()==2){
				string disease = toUpperCase(request[1]);
				totalCases(disease);
			}else{
				string location = toUpperCase(request[1]);
				string disease = toUpperCase(request[2]);
				casesInLocations(location, disease);
			}
		}else if(input == "help"){
			showHelpMenu();
		}else if(input =="exit"){
			break;
		}else{
			cout<<"Need help? Type Help' and press Enter key!"<<endl;	
		}
	}
	

	
	
	
	
	return 0;
	
}

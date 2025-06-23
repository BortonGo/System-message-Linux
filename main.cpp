#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;
using namespace std;

const int MAX_USERS = 100;
const int MAX_MESSAGES = 100;

class User {
public:
	string name;
	string login;
	string pass;

	void serialize(ofstream& out) const {
		out << name << '\n'<< login <<'\n' << pass << '\n';
	}
	bool deserialize(ifstream& in) {
		return getline(in,name) && getline(in,login) && getline(in,pass);
	}
};

class Message {
public:
	string text;
	string sender;
	string reciever;

	void serialize(ofstream& out) const {
		out << text << '\n' << sender << '\n' << reciever << '\n';
	}

	bool deserialize(ifstream& in) {
		return getline(in,text) && getline(in,sender) && getline(in,reciever);
	}
};

const string USERS_FILE = "users.txt";
const string MESSAGES_FILE = "messages.txt";

void setSecurePermissions(const string& filename) {
	fs::permissions(filename, fs::perms::group_all | fs::perms::others_all, fs::perm_options::remove);

	fs::permissions(filename, fs::perms::owner_read | fs::perms::owner_write, fs::perm_options::add);
}

void saveUsers(User users[], int count) {
	ofstream out (USERS_FILE);
	for (int i = 0; i < count; ++i) {
		users[i].serialize(out);
	}
	out.close();
	setSecurePermissions(USERS_FILE);
};
void saveMessages(Message messages[], int count) {
	ofstream out(MESSAGES_FILE);
	for (int i = 0; i < count; ++i) {
		messages[i].serialize(out);	
	}
	out.close();
	setSecurePermissions(MESSAGES_FILE);
}

int loadUsers(User users[], int maxUsers) {
	ifstream in(USERS_FILE);
	int count = 0;
	while(in && count < maxUsers) {
		if (users[count].deserialize(in)) {
			count++;
		}
	}
	in.close();
	return count;
}

int loadMessages(Message messages[], int maxMessages) {
	ifstream in(MESSAGES_FILE);
	int count = 0;
	while(in && count < maxMessages) {
		if (messages[count].deserialize(in)) {
			count++;
		}
	}
	in.close();
	return count;
}

int main() {
	User users[MAX_USERS];
	Message messages[MAX_MESSAGES];
	int userCount = loadUsers(users,MAX_USERS);
	int messageCount = loadMessages(messages, MAX_MESSAGES);

	cout << "System messages" << '\n';

	if(userCount < MAX_USERS) {
		users[userCount++] = {"Vanya", "vanya123", "qwerty"};
	}

	if(messageCount < MAX_MESSAGES) {
		messages[messageCount++] = {"Katya, privet", "Vanya", "Katya"};
	}

	saveUsers(users, userCount);
	saveMessages(messages, messageCount);

	cout << "The files are recorded. Access rights are set as rw-------.\n";

	return 0;
}

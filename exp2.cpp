#include <iostream>
#include<queue>
#include<vector>

using namespace std;

const int BUFFER_SIZE = 8;

vector<int> buffer(BUFFER_SIZE, 0);  //用数组模拟双端队列
queue<int> producer;  //empty queue
queue<int> consumer;  //full queue

int bufferEmpty;
int bufferFull;
int pid;   //模拟程序，假设pid和data相同
int ploc;
int cloc;

void init()
{
	bufferEmpty = BUFFER_SIZE;
	bufferFull = 0;
	pid = 1;
	ploc = 0;
	cloc = 0;
}

void show()
{
	cout << "BufferEmpty: " << bufferEmpty << endl;
	cout << "BufferFull: " << bufferFull << endl;

	cout << "buffer: ";
	for (int i = 0; i < buffer.size(); i++) {
		cout << buffer[i] << " ";
	}
	cout << endl;

	cout << "Empty Queue: ";
	for (int i = 0; i < producer.size(); i++) {
		cout << producer.front() << " ";
		producer.push(producer.front());
		producer.pop();
	}
	cout << endl;

	cout << "Full Queue: ";
	for (int i = 0; i < consumer.size(); i++) {
		cout << consumer.front() << " ";
		consumer.push(consumer.front());
		consumer.pop();
	}
	cout << endl;
}

bool isFull()
{
	bool tag = true;
	for (int i = 0; i < BUFFER_SIZE; i++) {
		if (buffer[i] == 0) {
			tag = false;
		}
	}

	return tag;
}

bool isEmpty()
{
	bool tag = true;
	for (int i = 0; i < BUFFER_SIZE; i++) {
		if (buffer[i] != 0) {
			tag = false;
		}
	}

	return tag;
}

void P()
{
	if (isFull()) {  //缓冲区满
		producer.push(pid);
		pid++;
	}
	else if (isEmpty()) {
		if (consumer.empty()) {
			buffer[ploc] = pid;
			pid++;
		}
		else {
			buffer[ploc] = pid;
			pid++;
			buffer[cloc] = 0;
			cloc = (cloc + 1) % BUFFER_SIZE;
			consumer.pop();
		}
		ploc = (ploc + 1) % BUFFER_SIZE;
	}
	else {
		buffer[ploc] = pid;
		pid++;
		ploc = (ploc + 1) % BUFFER_SIZE;
	}

	bufferEmpty--;
	bufferFull++;
}

void C()
{
	if (isEmpty()) {
		consumer.push(pid);
		pid++;
	}
	else if (isFull()) {
		if (!producer.empty()) {
			buffer[cloc] = 0;
			cloc = (cloc + 1) % BUFFER_SIZE;
			buffer[ploc] = producer.front();
			producer.pop();
			ploc = (ploc + 1) % BUFFER_SIZE;
		}
		else {
			buffer[cloc] = 0;
			cloc = (cloc + 1) % BUFFER_SIZE;
		}
	}
	else {
		buffer[cloc] = 0;
		cloc = (cloc + 1) % BUFFER_SIZE;
	}

	bufferEmpty++;
	bufferFull--;
}

void run()
{
	cout << "Please enter your action: e: exit p: produce c: consume" << endl;
	char action;
	while (cin >> action) {
		if (action == 'e') {
			break;
		}
		else if (action == 'p') {
			P();
			show();
		}
		else if (action == 'c') {
			C();
			show();
		}
		else {
			cout << "Wrong, please enter again" << endl;
		}
	}
}

int main()
{
	init();
	show();
	run();

	return 0;
}
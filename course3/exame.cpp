// Code start HERE
#include <deque>
#include <iostream>
// All includes that we need
#include <unistd.h> // sleep
#include <mutex> // mutex
#include <stdlib.h> // rand / srand
#include <time.h> // time
class CThread {

public:
static std::mutex deque_lock; // mutex for concurrency
bool finish = false;

virtual void Init(){}
virtual void Term(){finish = true;}
virtual void theThread(){}

void startThread(){
Init();
_thread = new low_level_thread_implementation(this, &theThread);
_thread->start();
}
void joinThread(){
Term();
_thread->join();
delete _thread;
}

int random_number(int low, int up) {
srand(time(NULL));;
return low + rand() % (up - low + 1);
}

private:
low_level_thread_implementation *_thread; // Provided by Library. Assume working
};
//
// Packet definition
//
struct SPacket {
unsigned int producer_id; // An unique Producer Id
unsigned int counter; // An incremental counter
size_t size; // Packet size
char *payload; // Up to 1500 bytes
};
//
// This class generate a stream of short packets
//
class CProducerShortPacket: public CThread {
public:
CProducerShortPacket(unsigned int producer_id, std::deque<SPacket>* packets): _packets(packets), _producer_id(_producer_id){}
~CProducerShortPacket(){}

void theThread(){
unsigned int counter = 0;
while (!finish) {
char buffer[1500];
// Forge Packet
SPacket packet;
packet.producer_id = _producer_id;
packet.counter = counter;
// Fill buffer with random numbers
packet.size = random_number(1, 64);
packet.payload = buffer;
for (auto i = 0; i < packet.size; i++) {
packet.payload[i] = random_number(0, 255);
}
deque_lock.lock();
_packets->push_back(packet);
deque_lock.unlock();
counter++;
}
}
private:
unsigned int _producer_id;
std::deque<SPacket>* _packets;
};
//
// This class generate a stream of long packets
//
class CProducerLongPacket: public CThread {
public:
CProducerLongPacket(unsigned int producer_id, std::deque<SPacket>* packets): _packets(packets), _producer_id(_producer_id)
{
}
~CProducerLongPacket()
{
}
/*void Init()
{
}
void Term()
{
// Nothing to do
}*/
void theThread()
{
unsigned int counter = 0;
while (!finish) {
// Forge Packet
SPacket packet;
packet.producer_id = _producer_id;
packet.counter = counter;
// Fill buffer with random numbers
//forge_long_packet(_producer_id, counter, packet); --> function does not exist
packet.size = random_number(1, 1500);
packet.payload = new char[packet.size];
for (auto i = 0; i < packet.size; i++) {
packet.payload[i] = random_number(0, 255);
}
deque_lock.lock();
_packets->push_back(packet);
deque_lock.unlock();
counter++;
}
}
private:
unsigned int _producer_id;
std::deque<SPacket>* _packets;
};
//
// This class filters the received all packets and prints on screen the sequence
// of packets of a given producer
//
class CConsumer: public CThread {
public:
CConsumer(unsigned int producer_id, std::deque<SPacket>* packets): _producer_id(producer_id), _packets(packets)
{
}
~CConsumer()
{
}
/*void Init()
{
}
void Term() override final
{
// Nothing to do
}*/
void theThread() final
{
unsigned int counter = 0;
while (!finish) {
if (!_packets->empty()) {
deque_lock.lock();
auto packet = _packets->front();
_packets->pop_front();
deque_lock.unlock();
if (packet.producer_id == _producer_id) {
std::cout << "Received packet #" << packet.counter <<
" from producer " << packet.producer_id << std::endl;
}
std::cout << "Total packet #" << ++counter << std::endl;
}
}
}
private:
unsigned int _producer_id;
std::deque<SPacket>* _packets;
};
int main(int argc, char **argv)
{
std::deque<SPacket> packets;
CProducerShortPacket producer1(1, &packets);
CProducerLongPacket producer2(2, &packets);
CProducerShortPacket producer3(3, &packets);
CConsumer consumer(2, &packets);
std::cout << "Simulation started!" << std::endl;
consumer.startThread();
producer1.startThread();
producer2.startThread();
producer3.startThread();
sleep(120); //sleep 120 seconds
consumer.joinThread();
producer1.joinThread();
producer2.joinThread();
producer3.joinThread();
std::cout << "Simulation completed!" << std::endl;
return 0;
}
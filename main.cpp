#include <thread>
#include <mutex>
#include <iostream>
#include <chrono>
#include <string>
#include <ncurses.h>
#include <unistd.h>
 
std::mutex mu;
 
void cleanScreeen(int numberOfPhilosopher)
{
    move(4,numberOfPhilosopher*25);
    printw("                        ");
    refresh();
 
    move(5,numberOfPhilosopher*25);
    printw("                        ");
    refresh();
}
 
void isOccuped(int numberOfFirstSpoon, int numberOfSecondSpoon, int ValueOfFirstSpoon, int ValueOfSecondSpoon, int numberOfPhilosopher)
{
    cleanScreeen(numberOfPhilosopher);
 
    if(ValueOfFirstSpoon==1)
    {
        move(4,numberOfPhilosopher*25);
        printw("Spoon:%d is occuped! ", numberOfFirstSpoon);
        refresh();
    }
       
    if(ValueOfSecondSpoon==1)
    {
        move(5,numberOfPhilosopher*25);
        printw("Spoon:%d is occuped! ", numberOfSecondSpoon);
        refresh();
    }
}
 
void starting (int numberOfPhilosopher)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(100*numberOfPhilosopher));
    move(1,numberOfPhilosopher*25);
    printw("--------START----------");
    refresh();
}
 
void ending (int numberOfPhilosopher)
{
    cleanScreeen(numberOfPhilosopher);
    move(3,numberOfPhilosopher*25);
    printw("--------END------------");
    refresh();
}
 
void eating(int numberOfPhilosopher)
{
    move(2,numberOfPhilosopher*25);
    printw("Is eating! 25        ");
    refresh();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000)); //sleep
    move(2,numberOfPhilosopher*25);
    printw("Is eating! 50        ");
    refresh();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000)); //sleep
    move(2,numberOfPhilosopher*25);
    printw("Is eating! 75        ");
    refresh();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000)); //sleep
    move(2,numberOfPhilosopher*25);
    printw("Is eating! 100       ");
    refresh();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000)); //sleep
}
 
void thinking(int numberOfPhilosopher)
{
    move(2,numberOfPhilosopher*25);
    printw("Is thinking! 25        ");
    refresh();
    std::this_thread::sleep_for(std::chrono::milliseconds(750)); //sleep
    move(2,numberOfPhilosopher*25);
    printw("Is thinking! 50        ");
    refresh();
    std::this_thread::sleep_for(std::chrono::milliseconds(750)); //sleep
    move(2,numberOfPhilosopher*25);
    printw("Is thinking! 75        ");
    refresh();
    std::this_thread::sleep_for(std::chrono::milliseconds(750)); //sleep
    move(2,numberOfPhilosopher*25);
    printw("Is thinking! 100       ");
    refresh();
    std::this_thread::sleep_for(std::chrono::milliseconds(750)); //sleep
}
 
void Philosopher(int firstSpoon, int secondSpoon, int Spoon[5], int numberOfPhilosopher, int hungry[5], int maxHungry, int status[5]) {
 
    int countermH=0, countH=0;
    int nextPhilosopher, backPhilosopher, mainPhilosopher;
    mainPhilosopher=numberOfPhilosopher-1;
    if(numberOfPhilosopher==1)
    {
        backPhilosopher=4;
    }
    else
    {
        backPhilosopher=numberOfPhilosopher-2;
    }
    mainPhilosopher=numberOfPhilosopher-1;
    if(numberOfPhilosopher==5)
    {
        nextPhilosopher=0;
    }
    else
    {
        nextPhilosopher=numberOfPhilosopher;
    }
 
    starting(numberOfPhilosopher);
    for(int i=0; i<4; i++)
    {
        status[mainPhilosopher]=1;
        for(;(((Spoon[firstSpoon])==1)||((Spoon[secondSpoon])==1));)
        {
            isOccuped(firstSpoon, secondSpoon, Spoon[firstSpoon], Spoon[secondSpoon], numberOfPhilosopher);
            move(2,numberOfPhilosopher*25);
            printw("Is waiting! Hungry: %d ", hungry[mainPhilosopher]);
            refresh();
            hungry[mainPhilosopher]++;
            std::this_thread::sleep_for(std::chrono::milliseconds(1000)); //sleep
        }
        status[mainPhilosopher]=0;
        cleanScreeen(numberOfPhilosopher);
        mu.lock();
        maxHungry=0;
        for(int j=0; j<5; j++)
        {
            if(maxHungry<hungry[j]) maxHungry=hungry[j];
        }
        if((((status[backPhilosopher]==0)&&(status[nextPhilosopher]==0))||(maxHungry<=hungry[mainPhilosopher]))&&(Spoon[firstSpoon]==0)&&(Spoon[secondSpoon]==0))
            {
            Spoon[firstSpoon]=1;
            Spoon[secondSpoon]=1;
            status[mainPhilosopher]=1;
            mu.unlock();
            eating(numberOfPhilosopher);
            thinking(numberOfPhilosopher);
            hungry[mainPhilosopher]=0;
            Spoon[firstSpoon]=0;
            Spoon[secondSpoon]=0;
            status[mainPhilosopher]=0;
        }
        else
        {
            mu.unlock();
            move(2,numberOfPhilosopher*25);
            printw("Is waiting! Hungry: %d ", hungry[mainPhilosopher]);
            refresh();
            i--;
            hungry[mainPhilosopher]++;
            std::this_thread::sleep_for(std::chrono::milliseconds(1000)); //sleep
        }
    }
    ending(numberOfPhilosopher);
    hungry[mainPhilosopher]=0;
}
 
int main(int argc, char **argv) {
 
    initscr();
 
    int Spoon[5], hungry[5], status[5], maxHungry=0;
    for(int i=0; i<5; i++)
    {
        Spoon[i]=0;
        hungry[i]=10*i;
        status[i]=0;
    }
    std::thread Thread1{Philosopher, 0, 1, Spoon, 1, hungry, maxHungry, status};
    std::thread Thread2{Philosopher, 1, 2, Spoon, 2, hungry, maxHungry, status};
    std::thread Thread3{Philosopher, 2, 3, Spoon, 3, hungry, maxHungry, status};
    std::thread Thread4{Philosopher, 3, 4, Spoon, 4, hungry, maxHungry, status};
    std::thread Thread5{Philosopher, 4, 0, Spoon, 5, hungry, maxHungry, status};
    Thread1.join();
    Thread2.join();
    Thread3.join();
    Thread4.join();
    Thread5.join();
    getch();
    endwin();
}

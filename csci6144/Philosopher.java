/* Author: Sathish Ramamoorthy
 * Lab2  : The five dining philosophers problem
 * Course: CSCI6144
 *
 * This class solves the problem using semaphores
 */

import java.util.*;

class Philosopher extends Thread {

  static Dinner din;
	static Semaphore T = new Semaphore(4);
	static Semaphore fork[] = new Semaphore[5];

	int i;
	Random rand;

	Philosopher(int i) {
		this.i = i;
    rand = new Random(new Date().getTime() + (i * 1000));
		start();
	}

	public void run() {
		try {
			sleep(1000);
		} catch (InterruptedException ie) {}

		while(true) {
			think();
			T.waiT();
			fork[i].waiT();           din.setForkState(i, i, true);
			fork[(i+1) % 5].waiT();   din.setForkState(i, (i+1) % 5, true);
			eat();
			fork[(i+1) % 5].signal(); din.setForkState(i, (i+1) % 5, false);
			fork[i].signal();         din.setForkState(i, i, false);
			T.signal();
		}
	}
	void eat() {
    din.setEating(i);
		try {
			sleep((rand.nextInt(100) + 1) * 100);
		} catch (InterruptedException ie) {}
	}
	void think() {
    din.setThinking(i);
		try {
			sleep((rand.nextInt(100) + 1) * 100);
		} catch (InterruptedException ie) {}
    din.setWaiting(i);
	}
	public static void startThinkingAndEating(Dinner d) {
    din = d;
		for(int x=0; x<5; x++)
			fork[x] = new Semaphore(1);

		for(int x=0; x<5; x++)
			new Philosopher(x);
	}
}

/* Author: Sathish Ramamoorthy
 * Lab2  : Five dining philosophers & Producer consumer
 * Course: CSCI6144
 *
 * This is the generic semaphore class used to solve both
 * the problems for Lab2
 */

class Semaphore {

	int count;

  Semaphore(int i) {
    count = i;
  }

	synchronized void waiT() {
		count--;
		if (count < 0)
			try {
				wait();
			} catch (InterruptedException e) {}
	}

	synchronized void signal() {
		count++;
		if (count <= 0)
			notify();
	}
}

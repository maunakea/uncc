/* Author: Sathish Ramamoorthy
 * Lab2  : The producer consumer problem
 * Course: CSCI6144
 *
 * This class solves the problem using semaphores
 */

class ProducerConsumer {

  int k = 5;  // circular buffer size
  Semaphore S = new Semaphore(1);
  Semaphore N = new Semaphore(0);
  Semaphore E = new Semaphore(k);
  int b[] = new int[k];
  int in = 0;
  int out = 0;
  int v, w;
  Thread p, c;
	java.util.Random rand = new java.util.Random();
  ProduceConsume pc;

  ProducerConsumer(ProduceConsume pcr) {
    pc = pcr;

    Runnable Producer = new Runnable() {
    	public void run() {
        while(true) {
          v = produce();
          E.waiT();
          S.waiT();
          append(v);
          S.signal();
          N.signal();
        }
      }
    };
    Runnable Consumer = new Runnable() {
    	public void run() {
        while(true) {
          N.waiT();
          S.waiT();
          w = take();
          S.signal();
          E.signal();
          consume();
        }
      }
    };
    p = new Thread(Producer); p.start();
    c = new Thread(Consumer); c.start();
  }
  int produce() {
    pc.blankProducer();
    try {
      p.sleep(rand.nextInt(100) * 100);
		} catch (InterruptedException ie) {}
    int v = rand.nextInt(100);
    pc.produced(v);
    return v;
  }
  int take() {
    int x = b[out];
    pc.taken(out, x);
    out = (out + 1) % k;
    return x;
  }
  void append(int i) {
    b[in] = i;
    pc.append(i, in);
    in = (in + 1) % k;
  }
  void consume() {
    try {
      c.sleep((rand.nextInt(100) * 100) / 1);
      pc.blankConsumer();
      c.sleep((rand.nextInt(100) * 100) / 1);
      pc.consumerWait();
		} catch (InterruptedException ie) {}
  }
}

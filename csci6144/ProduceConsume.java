/* Author: Sathish Ramamoorthy
 * Lab2  : The producer consumer problem
 * Course: CSCI6144
 *
 * This class displays a graphical interface that
 * shows the producer and consumer
 */

import javax.swing.*;
import java.awt.event.*;
import java.awt.*;

class ProduceConsume extends JFrame {

  int k = 5;
  JTextField bTextField[] = new JTextField[k];
  JTextField producer = new JTextField(2);
  JTextField consumer = new JTextField(2);

  ProduceConsume() {
    enableEvents(AWTEvent.WINDOW_EVENT_MASK);
    setSize(250, 100);
    setTitle("Procucer Consumer");

    JPanel bufferPanel = new JPanel();
    for (int x = 0; x < k ; x++) {
      bTextField[x] = new JTextField(2);
      bTextField[x].setHorizontalAlignment(JTextField.CENTER);
      bTextField[x].setEnabled(false);
      bufferPanel.add(bTextField[x]);
    }
    producer.setHorizontalAlignment(JTextField.CENTER);
    consumer.setHorizontalAlignment(JTextField.CENTER);
    producer.setEnabled(false);
    consumer.setEnabled(false);

    JPanel bot = new JPanel();
    bot.add(producer);
    bot.add(new JLabel(" < Producer    "));
    bot.add(new JLabel("    Consumer > "));
    bot.add(consumer);

    getContentPane().add(bufferPanel, BorderLayout.NORTH);
    getContentPane().add(bot, BorderLayout.SOUTH);
    new ProducerConsumer(this);
    setVisible(true);
  }

  void produced(int i) {
    producer.setText("" + i);
    producer.setBackground(Color.yellow);
  }
  void append(int i, int in) {
    bTextField[in].setText(""+i);
    bTextField[in].setBackground(Color.green);
  }
  void taken(int i, int x) {
    bTextField[i].setText("");
    bTextField[i].setBackground(Color.white);
    consumer.setText(""+x);
    consumer.setBackground(Color.green);
  }
  void blankProducer() {
    producer.setText("");
    producer.setBackground(Color.white);
  }
  void blankConsumer() {
    consumer.setText("");
    consumer.setBackground(Color.white);
  }
  void consumerWait() {
    consumer.setText("");
    consumer.setBackground(Color.yellow);
  }
  protected void processWindowEvent(WindowEvent e) {
    super.processWindowEvent(e);
    if(e.getID() == WindowEvent.WINDOW_CLOSING) {
      System.exit(0);
    }
  }
  public static void main(String args[]) {
    new ProduceConsume();
  }
}

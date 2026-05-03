/* Author: Sathish Ramamoorthy
 * Lab2  : The five dining philosophers problem
 * Course: CSCI6144
 *
 * This class displays a graphical interface that
 * shows the philosophers dining
 */

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.awt.geom.*;

class Dinner extends JFrame {

    Ellipse2D.Double phil[] = new Ellipse2D.Double[5];
    Line2D.Double fork[] = new Line2D.Double[5];
    Color head[] = new Color[5];
    Point2D.Double philPoint[] = new Point2D.Double[5];
    Point2D.Double forkPoint[] = new Point2D.Double[5];
    boolean forkState[][] = new boolean[5][2];

    Dinner() {
      enableEvents(AWTEvent.WINDOW_EVENT_MASK);
      setSize(300, 300);
      setTitle("Five dining Philosophers");
      setBackground(Color.lightGray);

      for (int x=0; x<5; x++)
        head[x] = Color.red;

      phil[0] = new Ellipse2D.Double(135.0, 56.0, 25.0, 25.0);
      phil[1] = new Ellipse2D.Double(50.0, 120.0, 25.0, 25.0);
      phil[2] = new Ellipse2D.Double(80.0, 220.0, 25.0, 25.0);
      phil[3] = new Ellipse2D.Double(193.0, 220.0, 25.0, 25.0);
      phil[4] = new Ellipse2D.Double(225.0, 120.0, 25.0, 25.0);

      fork[0] = new Line2D.Double(150.0, 230.0, 150.0, 197.0);
      fork[1] = new Line2D.Double(215.0, 185.0, 190.0, 170.0);
      fork[2] = new Line2D.Double(190.0, 100.0, 170.0, 125.0);
      fork[3] = new Line2D.Double(110.0, 100.0, 125.0, 125.0);
      fork[4] = new Line2D.Double(85.0, 185.0, 115.0, 170.0);

      philPoint[0] = new Point2D.Double(147.0, 68.0);
      philPoint[1] = new Point2D.Double(62.0, 131.0);
      philPoint[2] = new Point2D.Double(92.0, 232.0);
      philPoint[3] = new Point2D.Double(206.0, 232.0);
      philPoint[4] = new Point2D.Double(237.0, 132.0);

      forkPoint[0] = new Point2D.Double(182.0, 110.0);
      forkPoint[1] = new Point2D.Double(116.0, 109.0);
      forkPoint[2] = new Point2D.Double(99.0, 178.0);
      forkPoint[3] = new Point2D.Double(150.0, 214.0);
      forkPoint[4] = new Point2D.Double(203.0, 177.0);

      setVisible(true);
    }

    public void paint (Graphics g) {
      Graphics2D g2 = (Graphics2D) g;
      g2.draw(new Ellipse2D.Double(75.0, 85.0, 150.0, 150.0));

      for (int x=0; x<5; x++) {
        g2.setPaint(head[x]);
        g2.fill(phil[x]);
      }

      g2.setPaint(Color.black);
      for (int x=0; x<5; x++) {
        g2.draw(fork[x]);
      }

      for (int x=0; x<5; x++) {
        if (forkState[x][0])
          g2.setPaint(Color.blue);
        else
          g2.setPaint(Color.lightGray);
        g2.draw(new Line2D.Double(philPoint[x], forkPoint[x]));

        if (forkState[x][1])
          g2.setPaint(Color.blue);
        else
          g2.setPaint(Color.lightGray);
        g2.draw(new Line2D.Double(philPoint[x], forkPoint[(x + 1) % 5]));
      }

    }
    synchronized void setForkState(int p, int f, boolean b) {
      if (p == f) {
        forkState[p][0] = b;
      } else {
        forkState[p][1] = b;
      }
      repaint();
    }
    synchronized void setThinking(int i) {
      head[i] = Color.red;
      repaint();
    }
    synchronized void setEating(int i) {
      head[i] = Color.green;
      repaint();
    }
    synchronized void setWaiting(int i) {
      head[i] = Color.yellow;
      repaint();
    }
    protected void processWindowEvent(WindowEvent e) {
      super.processWindowEvent(e);
      if(e.getID() == WindowEvent.WINDOW_CLOSING) {
        System.exit(0);
      }
    }
    public static void main(String[] args) {
      Dinner d = new Dinner();
      Philosopher.startThinkingAndEating(d);
    }
}

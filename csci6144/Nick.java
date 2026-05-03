/* Author: Sathish Ramamoorthy
 * Lab3  : Client/Server programming
 * Course: CSCI6144
 *
 * This is the server that can serve mutliple clients
 * using sockets
 */

import java.net.*;
import java.io.*;
import java.util.*;

class Nick {

	Nick(int port) throws IOException {
		ServerSocket serversocket = new ServerSocket(port);
		System.out.println("Server Nick started on port " + port);
		try {
			while(true) {
				Socket socket = serversocket.accept();
				try {
					new NickServer(socket);
				} catch (IOException ioe) {
					socket.close();
				}
			}
		} finally {
			serversocket.close();
		}
	}

	public static void main(String args[]) throws IOException {
		new Nick(Integer.parseInt(args[0]));
	}
}

class NickServer extends Thread {

	Socket socket;
	BufferedReader in;
	PrintWriter out;
	static String hostname;
	static Vector messages;

	NickServer(Socket s) throws IOException {
		if (messages == null)
			messages = new Vector();
		if (hostname == null)
			hostname = s.getLocalAddress().getHostName();
		socket = s;
		System.out.println("New client service: " + s);
		in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
		out = new PrintWriter(new BufferedWriter(new OutputStreamWriter(
								  socket.getOutputStream())), true);
		start();
	}

	public void run() {
		try {
			while (true) {
				String str = in.readLine();
				String response = "bad request";
				if (str.equalsIgnoreCase("exit")) {
					out.println("Goodbye");
					break;
				} else if (str.equalsIgnoreCase("huh?")) {
					response = "Connected to Nick on " + hostname + "\n";
					if (messages.isEmpty())
						response += "No messages available";
					else if (messages.size() == 1)
						response += "1 message available";
					else
						response += "Messages are 1-" + messages.size();
				} else if (str.startsWith("tell ")) {
					System.out.println("Got a new message, number: " + (messages.size()+1));
					String s = str.substring(5);
					messages.add(s);
					response = "That is message " + messages.size() + " on " + hostname;
				} else if (str.startsWith("ask ")) {
					String s = str.substring(4);
					try {
						int i = Integer.parseInt(s);
						if (i > messages.size())
							response = "Message " + i + " not available";
						else
							response = "Message " + i + " from " + hostname + "\n" + (String)(messages.elementAt(i-1));
					} catch(NumberFormatException nfe) {
					}
				}
				System.out.println(str);
				out.println(response);
				out.println("end");
			}
			System.out.println("closing...");
		} catch(IOException e) {
			System.err.println("IO Exception");
		} finally {
			try {
				socket.close();
			} catch(IOException e) {
				System.err.println("Socket not closed");
			}
		}
	}
}

/* Author: Sathish Ramamoorthy
 * Lab3  : Client/Server programming
 * Course: CSCI6144
 *
 * This is the client program using RMI
 */

import java.rmi.*;
import java.rmi.registry.*;
import java.io.*;

class TellNickRmi {
	
	public static void main(String args[]) throws Exception {
		int port = Integer.parseInt(args[1]);
		NickRmiI nick = (NickRmiI)Naming.lookup(
			"//" + args[0] + ":" + port + "/NickRmi");
		BufferedReader userInput = new BufferedReader(new InputStreamReader(System.in));
		String input, s;
		System.out.print("> ");
		while ((input = userInput.readLine()) != null) {
			if (input.length() == 0) {
				System.out.print("> ");
				continue;
			} else if (input.startsWith("tell ")) {
				s = input.substring(5);
				System.out.println(nick.tell(s));
			} else if (input.startsWith("ask ")) {
				s = input.substring(4);
				try {
					int i = Integer.parseInt(s);
					System.out.println(nick.ask(i));
				} catch(NumberFormatException nfe) {
					System.out.println("bad number");
				}
			} else if (input.equalsIgnoreCase("huh?")) {
				System.out.println(nick.huh());
			} else if (input.equalsIgnoreCase("exit")) {
				System.out.println(nick.exit());
				System.exit(0);
			} else {
				System.out.println("bad request");
			}
			System.out.print("> ");
		}
	}
}

/* Author: Sathish Ramamoorthy
 * Lab3  : Client/Server programming
 * Course: CSCI6144
 *
 * This is the client program using sockets
 */

import java.net.*;
import java.io.*;

class TellNick {

	public static void main(String args[]) throws IOException {
		InetAddress addr = InetAddress.getByName(args[0]);
		System.out.println("addr = " + addr);
		Socket socket = new Socket(addr, Integer.parseInt(args[1]));
		try {
			System.out.println("socket = " + socket);
			BufferedReader in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
			PrintWriter out = new PrintWriter(new BufferedWriter(new OutputStreamWriter(
												socket.getOutputStream())),true);
			BufferedReader userInput = new BufferedReader(new InputStreamReader(System.in));
			String input;
			System.out.print("> ");
			while ((input = userInput.readLine()) != null) {
				if (input.length() == 0) {
					System.out.print("> ");
					continue;
				} else if (input.equalsIgnoreCase("exit")) {
					out.println("exit");
					System.out.println(in.readLine());
					System.exit(0);
				} else {
					out.println(input);
					String resp = "";
					while(!(resp = in.readLine()).equals("end")) {
						System.out.println(resp);
					}
				
				}
				System.out.print("> ");
			}
		} finally {
			System.out.println("closing...");
			socket.close();
		}
	}
}

/* Author: Sathish Ramamoorthy
 * Lab3  : Client/Server programming
 * Course: CSCI6144
 *
 * This is the server that can serve mutliple clients
 * using RMI
 */

import java.rmi.*;
import java.rmi.registry.*;
import java.rmi.server.*;
import java.net.*;
import java.util.*;

class NickRmi extends UnicastRemoteObject implements NickRmiI {

	static String hostname;
	Vector messages = new Vector();

	public String ask(int i) throws RemoteException {
		String response;
		if (i > messages.size())
			response = "Message " + i + " not available";
		else
			response = "Message " + i + " from " + hostname + "\n" + (String)(messages.elementAt(i-1));
		return response;
	}

	public String tell(String s) throws RemoteException {
		System.out.println("Got a new message, number: " + (messages.size()+1));
		System.out.println(s);
		messages.add(s);
		return ("That is message " + messages.size() + " on " + hostname);
	}

	public String huh() throws RemoteException {
		String response;
		response = "Connected to Nick on " + hostname + "\n";
		if (messages.isEmpty())
			response += "No messages available";
		else if (messages.size() == 1)
			response += "1 message available";
		else
			response += "Messages are 1-" + messages.size();
		return response;
	}
	public String exit() throws RemoteException {
		System.out.println("a client exiting");
		return "Goodbye.";
	}
	public NickRmi() throws RemoteException {
	}
	public static void main(String[] args) throws Exception {
		System.out.print("Starting registry... ");
		int port = Integer.parseInt(args[1]);
		LocateRegistry.createRegistry(port);
		System.out.println("done");
		//System.setSecurityManager(new RMISecurityManager());
		NickRmi nick = new NickRmi();
		hostname = args[0];
		Naming.bind("//" + hostname + ":" + port + "/NickRmi", nick);
		System.out.println("Nick on localhost started on port " + port + ".");
	}
}

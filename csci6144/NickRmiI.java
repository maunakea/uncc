/* Author: Sathish Ramamoorthy
 * Lab3  : Client/Server programming
 * Course: CSCI6144
 *
 * This is the RMI interface
 */

import java.rmi.*;

interface NickRmiI extends Remote {

	String ask(int i) throws RemoteException;

	String tell(String s) throws RemoteException;

	String huh() throws RemoteException;

	String exit() throws RemoteException;

}

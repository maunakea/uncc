/* Author:  Sathish Ramamoorthy
 * Course:  CSCI6144
 * Lab1:    Tiny command Interpreter in Java
 */
import java.io.*;

public class Lab1 {
  public static void main(String[] args) throws IOException {

	  BufferedReader stdInput, stdError, cmdInput;
	  Process process = null;
	  String cmd, s;
    cmdInput = new BufferedReader(new InputStreamReader(System.in));
    System.out.print("Tiny Java command interpreter started...\nos> ");

	  while ((cmd = cmdInput.readLine()) != null) {
	    try {
		    if (cmd.length() == 0)
		      continue;
		    else if (cmd.equalsIgnoreCase("exit")) {
		      System.out.println("Tiny Java command interpreter exiting.");
		      System.exit(0);
		    }

		    // process is created
        process = Runtime.getRuntime().exec(cmd);

        // the standard input and error stream from the child process
        // the child's std output will be read as std input by the parent
        stdInput = new BufferedReader(new InputStreamReader(process.getInputStream()));
        stdError = new BufferedReader(new InputStreamReader(process.getErrorStream()));

        // the ouput from the child and write to parent's standard output
        while ((s = stdInput.readLine()) != null) {
          System.out.println(s);
        }
        while ((s = stdError.readLine()) != null) {
          System.out.println(s);
        }
        // destroy the child process and close streams
	      process.destroy();
        stdInput.close();
	      stdError.close();
	    } catch(IOException ioe) {
		    System.out.println("Not a valid command.");
	    } finally {System.out.print("os> ");}
	  }
  }
}


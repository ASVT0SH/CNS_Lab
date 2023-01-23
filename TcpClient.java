import java.net.Socket;
import java.io.IOException;
import java.io.PrintStream;
import java.util.Scanner;

class Client {

    public static void main(String[] args) {
        try {
            Socket socket = new Socket("127.0.0.1", 1300);
            Scanner socketScanner = new Scanner(socket.getInputStream());
            Scanner consoleScanner = new Scanner(System.in);
            System.out.println("Enter File Name");
            String fileName = consoleScanner.nextLine();
            PrintStream printStream = new PrintStream(socket.getOutputStream());
            printStream.println(fileName);
            while (socketScanner.hasNextLine()) {
                System.out.println(socketScanner.nextLine());
            }
            consoleScanner.close();
            socketScanner.close();
            socket.close();
        } catch (IOException e) {
            System.out.println(e.getMessage());
        }

    }
}
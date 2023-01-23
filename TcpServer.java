import java.io.File;
import java.io.IOException;
import java.io.PrintStream;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.Scanner;

class TcpServer {
    public static void main(String[] args) {
        try {
            ServerSocket serverSocket = new ServerSocket(1300);
            Socket socket = serverSocket.accept();
            Scanner socketScanner = new Scanner(socket.getInputStream());
            String fileName = socketScanner.nextLine();
            PrintStream printStream = new PrintStream(socket.getOutputStream());
            File file = new File(fileName);
            if (file.exists()) {
                Scanner fileScanner = new Scanner(file);
                while (fileScanner.hasNextLine()) {
                    printStream.println(fileScanner.nextLine());
                }
                fileScanner.close();
            } else {
                printStream.println("ERROR: FILE DOESN'T EXISTS");
            }
            // Keep the server alive for complete transfer of data
            System.in.read();

            socket.close();
            serverSocket.close();
            socketScanner.close();
        } catch (IOException e) {

        }
    }
}
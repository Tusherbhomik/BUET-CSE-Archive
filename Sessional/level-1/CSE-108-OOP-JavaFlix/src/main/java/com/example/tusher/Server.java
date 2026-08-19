package com.example.tusher;

import java.io.*;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
public class Server {
    private static final String INPUT_FILE_NAME ="movies.txt";
    private static final String OUTPUT_FILE_NAME ="movies.txt";
    public static    List<Movie> allMovieList=new ArrayList<>();
    public static HashMap<String,SocketWrapper>clientMaps=new HashMap<>();
    public static String clientName;
    public Server() throws IOException {


        ServerSocket serverSocket = new ServerSocket(44444);


        while (true) {

            Socket clientSocket = serverSocket.accept();
            SocketWrapper socketWrapper=new SocketWrapper(clientSocket);
            new Thread(() -> {
                try {
                    while (true) {
                        Object data = socketWrapper.read();

                        if (data instanceof String) {

                            String str = (String) data;
                            String[] arr = str.split(",");
                             if (arr.length == 1) {
                                clientName = (String) data;
                                clientMaps.put(clientName.toUpperCase(), socketWrapper);
                                List<Movie> clientsMovie = new ArrayList<>();
                                for (int i = 0; i < allMovieList.size(); i++) {
                                    if (clientName.equalsIgnoreCase(allMovieList.get(i).getProductionCompany())) {
                                        clientsMovie.add(allMovieList.get(i));
                                    }
                                }
                                clientMaps.get(clientName.toUpperCase()).write(clientsMovie);

                            }
                            else if (arr.length == 3) {
                                for (int i = 0; i < allMovieList.size(); i++) {
                                    if (arr[0].equalsIgnoreCase(allMovieList.get(i).getName())) {
                                        allMovieList.get(i).setProductionCompany(arr[1]);
                                    }
                                }
                                try {
                                    writeToFile(allMovieList);
                                } catch (Exception e) {
                                    throw new RuntimeException(e);
                                }
                                List<Movie> senderMovieList = new ArrayList<>();
                                for (int i = 0; i < allMovieList.size(); i++) {
                                    if (arr[2].equalsIgnoreCase(allMovieList.get(i).getProductionCompany())) {
                                        senderMovieList.add(allMovieList.get(i));
                                    }
                                }
                                List<Movie> receiverMovieList = new ArrayList<>();
                                for (int i = 0; i < allMovieList.size(); i++) {
                                    if (arr[1].equalsIgnoreCase(allMovieList.get(i).getProductionCompany())) {
                                        receiverMovieList.add(allMovieList.get(i));
                                    }
                                }

                                List<Movie> list2 = new ArrayList<>(senderMovieList);
                                if(clientMaps.containsKey(arr[2].toUpperCase()))
                                    clientMaps.get(arr[2].toUpperCase()).write(list2);
                                if(clientMaps.containsKey(arr[1].toUpperCase()))
                                    clientMaps.get(arr[1].toUpperCase()).write(receiverMovieList);
                            }
                        }

                        if (data instanceof Movie) {
                            allMovieList.add((Movie) data);

                            writeToFile(allMovieList);
                            List<Movie> clientMovieList = new ArrayList<>();
                            for (int i = 0; i < allMovieList.size(); i++) {
                                if (clientName.equalsIgnoreCase(allMovieList.get(i).getProductionCompany()))
                                {
                                    clientMovieList.add(allMovieList.get(i));
                                }
                            }
                            clientMaps.get(clientName.toUpperCase()).write(clientMovieList);
                        }

                    }
                } catch (Exception e) {
                    throw new RuntimeException(e);
                } finally {
                    try {
                        clientMaps.get(clientName.toUpperCase()).closeConnection();
                    } catch (IOException e) {
                        throw new RuntimeException(e);
                    }
                }
            }).start();
        }

    }
    public static void main(String args[]) throws Exception {
        ReadFromFile();
         new Server();
    }
    public static void ReadFromFile() throws Exception {
        BufferedReader br = new BufferedReader(new FileReader(INPUT_FILE_NAME));
        while (true) {
            String line = br.readLine();
            if (line == null) break;
            String[] tokens = line.split(",");
            Movie temp = new Movie(tokens[0], tokens[1], tokens[2],tokens[3],
                    tokens[4], tokens[5], tokens[6],tokens[7],tokens[8]);


            allMovieList.add(temp);
        }
        br.close();
    }

    public static void writeToFile(List<Movie> allMovieList) throws Exception {
        BufferedWriter bw = new BufferedWriter(new FileWriter(OUTPUT_FILE_NAME));
        for (Movie m : allMovieList) {
            bw.write(m.getName()+","+m.getReleaseYear()+","+m.getGenre1()+","+m.getGenre2()+","+m.getGenre3()+","
                    +m.getRunningTime()+","+m.getProductionCompany()+","+m.getBudget()+","+m.getRevenue());
            bw.write("\n");
        }
        bw.close();
    }
}


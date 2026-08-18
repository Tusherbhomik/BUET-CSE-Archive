package com.example.tusher;

import javafx.application.Platform;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.TextField;
import javafx.stage.Stage;

import java.io.IOException;
import java.util.List;
import java.util.Objects;

public class Client {
    @FXML
    public static SocketWrapper socketWrapper;

    @FXML
    private TextField clientNameTextField;

    public static List<Movie> myMovieList;
    public static String productionCompany;
    public static Object object;
    public Client(Object o) throws IOException {
        object=o;

            try {
                socketWrapper=new SocketWrapper("127.0.0.1",44444);
            }catch (IOException e){
                throw new RuntimeException(e);
            }


        new Thread(()->
        {
            try {
                socketWrapper.write(object);
            } catch (IOException e) {
                throw new RuntimeException(e);
            }
        }
                ).start();

        new Thread(()->
        {
            while (true)
            {
                try {
                    Object oRead=socketWrapper.read();
                    myMovieList=(List<Movie>)oRead;


                   if(true)
                   {
                       Platform.runLater(new Runnable() {
                           @Override
                           public void run() {
                               if(TableController.usedTable ==null)return;
                               TableController.usedTable.getItems().clear();
                               for(Movie m:myMovieList)
                               {
                                   TableController.usedTable.getItems().add(m);
                               }
                           }
                       });
                   }
                } catch (IOException | ClassNotFoundException e) {
                    throw new RuntimeException(e);
                }
                }
            }).start();
    }
}
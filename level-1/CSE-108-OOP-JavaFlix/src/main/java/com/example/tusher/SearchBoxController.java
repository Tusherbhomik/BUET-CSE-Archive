package com.example.tusher;

import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.scene.Node;
import javafx.scene.Scene;
import javafx.scene.control.TextField;
import javafx.stage.Stage;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

public class SearchBoxController {
    @FXML
    private TextField movieNameTextField;
    @FXML
    private TextField movieNameTextField2;
    private Stage stage;
    private FXMLLoader fxmlLoader;
    private Scene scene;
    private List<Movie>findList=new ArrayList<>();
    public void onMovieSearchByTitle(ActionEvent actionEvent) throws IOException {

        String str;
        str=movieNameTextField.getText();

        for(int i=0;i<Client.myMovieList.size();i++)
        {
            if(str.equalsIgnoreCase(Client.myMovieList.get(i).getName()))
            {
                findList.add(Client.myMovieList.get(i));
            }
        }
//        for (int i=0;i<findList.size();i++)
//        {
//            System.out.println(findList.get(i).getName());
//        }

            if(findList.size()!=0){
                TableController.matchedMovieList=findList;
//                for (int i=0;i<findList.size();i++)
//                {
//                    System.out.println(TableController.matchedMovieList.get(i).getName());
//                }

                stage=(Stage)((Node)(actionEvent.getSource())).getScene().getWindow();
                fxmlLoader = new FXMLLoader(ClientLogin.class.getResource("table.fxml"));
                scene = new Scene(fxmlLoader.load());
                stage.setTitle("In Table View");
                stage.setScene(scene);
            }
            else{
                Stage stage=(Stage)((Node)(actionEvent.getSource())).getScene().getWindow();
                FXMLLoader fxmlLoader = new FXMLLoader(ClientLogin.class.getResource("error.fxml"));
                Scene scene = new Scene(fxmlLoader.load());
                stage.setTitle("Error");
                stage.setScene(scene);
            }
        }
    public void onMovieSearchByReleaseYear(ActionEvent actionEvent) throws IOException {
        boolean found=false;
        List<Movie> matchedList=new ArrayList<>();
        for(Movie m: Client.myMovieList){
            if(m.getReleaseYear().equalsIgnoreCase(movieNameTextField2.getText()))
            {
                found=true;
                //System.out.println(m.getName());
                matchedList.add(m);
            }
        }
        if(found){
            TableController.matchedMovieList=matchedList;
            stage=(Stage)((Node)(actionEvent.getSource())).getScene().getWindow();
            fxmlLoader = new FXMLLoader(ClientLogin.class.getResource("table.fxml"));
            scene = new Scene(fxmlLoader.load());
            stage.setTitle("In Table View");
            stage.setScene(scene);

        }
        else{
            Stage stage=(Stage)((Node)(actionEvent.getSource())).getScene().getWindow();
            FXMLLoader fxmlLoader = new FXMLLoader(ClientLogin.class.getResource("error.fxml"));
            Scene scene = new Scene(fxmlLoader.load());
            stage.setTitle("Error");
            stage.setScene(scene);
        }

    }
    public void onTitleBackClick(ActionEvent actionEvent) throws IOException {
        Stage stage=(Stage)((Node)(actionEvent.getSource())).getScene().getWindow();
        FXMLLoader fxmlLoader = new FXMLLoader(ClientLogin.class.getResource("info.fxml"));
        Scene scene = new Scene(fxmlLoader.load());
        stage.setTitle("Search Info");
        stage.setScene(scene);
    }
}

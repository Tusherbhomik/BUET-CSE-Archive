package com.example.tusher;

import javafx.event.ActionEvent;
import javafx.fxml.FXMLLoader;
import javafx.scene.Node;
import javafx.scene.Scene;
import javafx.stage.Stage;

import java.io.IOException;
public class MenuSceneController {
    public void onSearchByInfoClick(ActionEvent actionEvent) throws IOException {
        Stage stage=(Stage)((Node)(actionEvent.getSource())).getScene().getWindow();
        FXMLLoader fxmlLoader = new FXMLLoader(ClientLogin.class.getResource("info.fxml"));
        Scene scene = new Scene(fxmlLoader.load());
        stage.setTitle("Search Info");
        stage.setScene(scene);
    }
    public void onBackClick(ActionEvent actionEvent) throws IOException {
        Stage stage=(Stage)((Node)(actionEvent.getSource())).getScene().getWindow();
        FXMLLoader fxmlLoader = new FXMLLoader(ClientLogin.class.getResource("login.fxml"));
        Scene scene = new Scene(fxmlLoader.load());
        stage.setTitle("LogIn");
        stage.setScene(scene);
    }
    public void onNotFoundBackClick(ActionEvent actionEvent) throws IOException {

        Stage stage=(Stage)((Node)(actionEvent.getSource())).getScene().getWindow();
        FXMLLoader fxmlLoader = new FXMLLoader(ClientLogin.class.getResource("info.fxml"));
        Scene scene = new Scene(fxmlLoader.load());
        stage.setTitle("Error 404");
        stage.setScene(scene);

    }

    public void onclickAddMovie(ActionEvent actionEvent) throws IOException {
        Stage stage=(Stage)((Node)(actionEvent.getSource())).getScene().getWindow();
        FXMLLoader fxmlLoader = new FXMLLoader(ClientLogin.class.getResource("add-movie.fxml"));
        Scene scene = new Scene(fxmlLoader.load());
        stage.setTitle("Add Movie Scene");
        stage.setScene(scene);

    }

    public void onTransferMovieClick(ActionEvent actionEvent) throws IOException {
        Stage stage=(Stage)((Node)(actionEvent.getSource())).getScene().getWindow();
        FXMLLoader fxmlLoader = new FXMLLoader(ClientLogin.class.getResource("transfer.fxml"));
        Scene scene = new Scene(fxmlLoader.load());
        stage.setTitle("Transfer Movie Scene");
        stage.setScene(scene);
    }
}

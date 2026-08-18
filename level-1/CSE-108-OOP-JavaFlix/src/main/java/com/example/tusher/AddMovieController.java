package com.example.tusher;

import javafx.event.ActionEvent;
import javafx.fxml.FXMLLoader;
import javafx.scene.Node;
import javafx.scene.Scene;
import javafx.scene.control.TextField;
import javafx.stage.Stage;

import java.io.IOException;

public class AddMovieController {
    public TextField releaseYear;
    public TextField name;
    public TextField genre1;
    public TextField genre2;
    public TextField genre3;
    public TextField runningTime;
    public TextField budget;
    public TextField revenue;
    public void onAddMovieBackClick(ActionEvent actionEvent) throws IOException {
        Stage stage=(Stage)((Node)(actionEvent.getSource())).getScene().getWindow();
        FXMLLoader fxmlLoader = new FXMLLoader(ClientLogin.class.getResource("menu-scene.fxml"));
        Scene scene = new Scene(fxmlLoader.load());
        stage.setTitle("Add Movie to your list");
        stage.setScene(scene);
    }
    public void onConfirmClick(ActionEvent actionEvent) throws Exception {
        Stage stage=(Stage)((Node)(actionEvent.getSource())).getScene().getWindow();
        FXMLLoader fxmlLoader = new FXMLLoader(ClientLogin.class.getResource("confirmation.fxml"));
        Scene scene = new Scene(fxmlLoader.load());
        stage.setTitle("Add Movie to your list");
        stage.setScene(scene);
        Movie m=new Movie(name.getText(),releaseYear.getText(),genre1.getText(),genre2.getText(),
                genre3.getText()
                , runningTime.getText(), Client.productionCompany,budget.getText(),revenue.getText());
        Client.socketWrapper.write(m);

    }
    public void onBackAfterSuccessful(ActionEvent actionEvent) throws IOException {
        Stage stage=(Stage)((Node)(actionEvent.getSource())).getScene().getWindow();
        FXMLLoader fxmlLoader = new FXMLLoader(ClientLogin.class.getResource("menu-scene.fxml"));
        Scene scene = new Scene(fxmlLoader.load());
        stage.setTitle("Add Movie to your list");
        stage.setScene(scene);
    }
}

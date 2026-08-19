package com.example.tusher;

import javafx.application.Application;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.scene.Scene;
import javafx.stage.Stage;

public class ClientLogin extends Application {
    @FXML
    public static Stage  stage;
    @Override
    public void start(Stage stage) {
        this.stage =stage;
        try {
            FXMLLoader fxmlLoader = new FXMLLoader(ClientLogin.class.getResource("login.fxml"));

            Scene scene = new Scene(fxmlLoader.load());
            stage.setTitle("JAVAFLIX");
            stage.setScene(scene);
            stage.show();
        }
        catch(Exception e)
        {
            System.out.println(e);
        }
    }
    public static void main(String[] args) {
        launch();
    }
}
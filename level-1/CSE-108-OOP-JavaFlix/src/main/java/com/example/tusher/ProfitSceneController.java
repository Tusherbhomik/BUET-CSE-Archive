package com.example.tusher;

import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.fxml.Initializable;
import javafx.scene.Node;
import javafx.scene.Scene;
import javafx.scene.control.Label;
import javafx.stage.Stage;

import java.io.IOException;
import java.net.URL;
import java.util.ResourceBundle;

public class ProfitSceneController implements Initializable {
    @FXML
    private Label profitLabel;
    public static long total;
        public void onTitleBackClick(ActionEvent actionEvent) throws IOException {
            Stage stage=(Stage)((Node)(actionEvent.getSource())).getScene().getWindow();
            FXMLLoader fxmlLoader = new FXMLLoader(ClientLogin.class.getResource("info.fxml"));
            Scene scene = new Scene(fxmlLoader.load());
            stage.setTitle("Search Info");
            stage.setScene(scene);
        }

    @Override
    public void initialize(URL location, ResourceBundle resources) {
            profitLabel.setText("Total Profit :"+String.valueOf(total));

    }
}


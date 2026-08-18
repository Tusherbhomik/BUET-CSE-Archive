package com.example.tusher;

import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.fxml.Initializable;
import javafx.scene.Node;
import javafx.scene.Scene;
import javafx.scene.control.TableColumn;
import javafx.scene.control.TableView;
import javafx.scene.control.cell.PropertyValueFactory;
import javafx.stage.Stage;

import java.io.IOException;
import java.net.URL;
import java.util.List;
import java.util.ResourceBundle;

public class TableController implements Initializable {
    @FXML
    private  TableView<Movie> table;
    public static  TableView<Movie>usedTable;
    @FXML private  TableColumn<Movie, String> Name;
    @FXML private  TableColumn<Movie, String> releaseYear;
    @FXML private  TableColumn<Movie,String> genre1;
    @FXML private   TableColumn<Movie,String> genre2;
    @FXML private   TableColumn<Movie,String> genre3;
    @FXML private  TableColumn<Movie,String> runningTime;

    @FXML private  TableColumn<Movie, String> productionCompany;
    @FXML private  TableColumn<Movie, String> budget;
    @FXML private  TableColumn<Movie, String> revenue;
    public static List<Movie>matchedMovieList;
    public static int start;
    public void ontableBackClick(ActionEvent actionEvent) throws IOException {
        start=0;
        Stage stage=(Stage)((Node)(actionEvent.getSource())).getScene().getWindow();
        FXMLLoader fxmlLoader = new FXMLLoader(ClientLogin.class.getResource("info.fxml"));
        Scene scene = new Scene(fxmlLoader.load());
        stage.setTitle("Search Info");
        stage.setScene(scene);
    }

    @Override
    public  void initialize(URL location, ResourceBundle resources) {
        start=1;
        ObservableList<Movie> observableMovieList= FXCollections.observableArrayList(matchedMovieList);
        Name.setCellValueFactory(new PropertyValueFactory<Movie,String>("Name"));
        releaseYear.setCellValueFactory(new PropertyValueFactory<Movie,String>("releaseYear"));
        genre1.setCellValueFactory(new PropertyValueFactory<Movie,String>("genre1"));
        genre2.setCellValueFactory(new PropertyValueFactory<Movie,String>("genre2"));
        genre3.setCellValueFactory(new PropertyValueFactory<Movie,String>("genre3"));
        runningTime.setCellValueFactory(new PropertyValueFactory<Movie,String>("runningTime"));
        productionCompany.setCellValueFactory(new PropertyValueFactory<Movie,String>("productionCompany"));
        budget.setCellValueFactory(new PropertyValueFactory<Movie,String>("budget"));
        revenue.setCellValueFactory(new PropertyValueFactory<Movie,String>("revenue"));
        table.setItems(observableMovieList);
        usedTable=table;
    }
}

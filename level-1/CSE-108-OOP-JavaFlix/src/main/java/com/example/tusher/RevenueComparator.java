package com.example.tusher;

import java.util.Comparator;

public class RevenueComparator implements Comparator<Movie> {

    @Override
    public int compare(Movie o1, Movie o2) {
        return o2.get_Revenue()- o1.get_Revenue();
    }
}
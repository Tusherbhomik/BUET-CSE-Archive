package com.example.tusher;

import java.util.Comparator;

public class YearComparator implements Comparator<Movie>{

        @Override
        public int compare(Movie o1, Movie o2) {
            return o2.get_year()- o1.get_year();
        }
    }

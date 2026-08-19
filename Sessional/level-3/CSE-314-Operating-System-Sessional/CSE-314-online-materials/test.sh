#!/bin/bash
exec < c.txt
exec < b.txt
exec < a.txt
read line
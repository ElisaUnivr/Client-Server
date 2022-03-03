#!/bin/bash

IFS="/"



grep home /etc/passwd | cut -d ':' -f 1,6
	

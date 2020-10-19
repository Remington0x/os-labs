#!/bin/bash

kill -9 $(ps | grep lab2 | awk '{print $1}')

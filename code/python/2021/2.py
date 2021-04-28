#!/usr/bin/python
# -*- coding: UTF-8 -*-
 
def fab(max): 
    n, a, b = 0, 0, 1 
    while n < max: 
        print(b) 
        a, b = b, a + b 
        n = n + 1
fab(5)
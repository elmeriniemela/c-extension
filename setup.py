#!/usr/bin/env python3.7

from distutils.core import setup, Extension

module1 = Extension('myModule',
                    sources = ['hello.c'])

setup (name = 'myModule',
       version = '1.0',
       description = 'This is a demo package',
       ext_modules = [module1])
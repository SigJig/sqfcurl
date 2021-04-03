cmake_minimum_required(VERSION 3.10)

project(sqfcurl VERSION 1.0)

ExternalProject_Add(
    curlpp
    GIT REPOSITORY https://github.com/jpbarrette/curlpp
)
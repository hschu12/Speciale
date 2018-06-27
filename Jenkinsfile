pipeline {
  agent any
  stages {
    stage('Build') {
      steps {
        sh '''cd src
g++ -std=c++14 -O3 src/PaperTest.cpp -o PaperTest'''
      }
    }
  }
}
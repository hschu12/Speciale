pipeline {
  agent any
  stages {
    stage('Build') {
      steps {
        sh '''make
'''
      }
    }
    stage('Run Tests') {
      steps {
        sh '''./CycleTest
./DeadEndTest
./PaperTest
./ThesisTest
./ThesisTestWithCycles
'''
      }
    }
    stage('') {
      steps {
        echo 'All done'
      }
    }
  }
}
pipeline {
  agent {
    docker 'gcc:latest'
  }
  stages {
    stage('Build') {
      steps {
        sh 'make --version'
      }
    }
  }
}
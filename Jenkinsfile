pipeline {
  agent {
    docker {
      image 'gcc:latest'
    }
    
  }
  stages {
    stage('Build') {
      steps {
        sh 'make --version'
      }
    }
  }
}
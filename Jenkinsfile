pipeline { 
  agent { docker 'gcc:latest' } 
  stages { 
    stage('build') { 
      steps { 
        sh 'make --version' } 
    } 
  } 
}

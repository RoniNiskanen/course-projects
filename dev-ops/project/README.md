This repository includes DevOps course project source code.

For the project, a local GitLab instance was set up as a container. 
A local GitLab instance was needed so that an own runner could be registered. 
When the GitLab instance was up and running, a pipeline was built and used to implement the needed changes to the system.
The pipeline had four stages shown below in a picture.

![Pipeline](https://github.com/RoniNiskanen/course-projects/blob/main/dev-ops/project/readme_images/pipeline.png?raw=true "Pipeline")

Test-driven development was used to implement the changes to the system. 
Pytest was used as a testing tool, and flake8 was used as a linter and ran on the static tests step. 
The system is deployed to the local machine, and it consists of several Docker containers that communicate with each other using AMQP and HTTP.
Information about the system can be fetched and the state changed using the API gateway.

![System](https://github.com/RoniNiskanen/course-projects/blob/main/dev-ops/project/readme_images/system.png?raw=true "System")

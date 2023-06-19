FROM python:3.9.17-alpine

# for flask web server
EXPOSE 5000
EXPOSE 8883
EXPOSE 1883

# set working directory
WORKDIR /app

# install required libraries
COPY requirements.txt .
RUN pip install -r requirements.txt

# copy source code into working directory
COPY code /app

# This is the runtime command for the container
CMD python3 -m flask run
# lsbsystem
Embedded application for home env monitoring
<br>
<br>

## Intro
<div>
	<img src="https://img.shields.io/badge/Linux-FCC624?style=flat&logo=Linux&logoColor=black" />
	<img src="https://img.shields.io/badge/C-A8B9CC?style=flat&logo=C&logoColor=black" />
	<img src="https://img.shields.io/badge/C++-00599C?style=flat&logo=cplusplus&logoColor=black" />
	<img src="https://img.shields.io/badge/go-00ADD8?style=flat&logo=go&logoColor=black" />
</div>
This is an embedded software project for Raspberry Pi 4B<br><br>



## Dev Environment
I tested this project on Ububtu 22.04 LTS<br>
Cross compile is also possible with buildroot<br>
<br>

## Installation
Type command below on your terminal

	git clone https://github.com/dlatjdqo34/lsbsystem.git
 	cd lsbsystem

## How to build
This project used Cmake to build<br>
You can simply build the project by execute shell script named "__build__" <br>
This shell script would make output directory contains Cmake relational files and executable file. Then move exe file to the working directory<br><br>
Just Type the command below in project directory

	./build		# build whole project with shell script using Cmake
 	./lsbsystem	# Simpley run program by execute ELF file 



 


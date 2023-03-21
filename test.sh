#!/usr/bin/env bash
#testy pro IPk projekt 1

function echo_fail(){
	echo -e "          [\033[1;31mFail\033[0m]"
}
function echo_pass(){
	echo -e "          [\033[1;32mPass\033[1;0m]"
}
binfile='./ipkcpc.out';
host='localhost';
portudp='2023';
porttcp='2024';


function testudp(){ # $1 - name
	echo "test $1:";
	echo "---------------";
	wcount=$(cat `echo "testfiles/$1.in"` | `echo "$binfile -h $host -p $portudp -m udp"`| diff testfiles/`echo $1`.out - );
	if [[ -z "$wcount" ]]
	then
		echo_pass;
	else
		cat `echo "testfiles/$1.in"` | `echo "$binfile -h $host -p $portudp -m udp"`;
		echo_fail;
	fi
}

function testtcp(){ # $1 - name
	echo "test $1:";
	echo "---------------";
	wcount=$(cat `echo "testfiles/$1.in"` | `echo "$binfile -h $host -p $porttcp -m tcp"`| diff testfiles/`echo $1`.out - );
	if [[ -z "$wcount" ]]
	then
		echo_pass;
	else
		cat `echo "testfiles/$1.in"` | `echo "$binfile -h $host -p $porttcp -m tcp"`;
		echo_fail;
	fi
}

testudp udp;
testudp udpdivZ;
testudp udpInvReq;

testtcp tcp;
testtcp tcpdivZ;
testtcp tcpInvReq;

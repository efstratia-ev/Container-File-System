#include "cfs_elmnt.h"

cfs_elmnt::cfs_elmnt(unsigned int fnsz){
	filename=new char[fnsz];
}
cfs_elmnt::~cfs_elmnt(){
	delete filename;
}

void cfs_elmnt::print(){
	cout<<"nodeid: "<<nodeid<<endl;
	cout<<"filename: "<<filename<<endl;
	cout<<"size: "<<size<<endl;
	cout<<"type: "<<type<<endl;
	cout<<"pnid: "<<parent_nodeid<<endl;
	cout<<"c_time: "<<creation_time<<endl;
	cout<<"a_time: "<<access_time<<endl;
	cout<<"m_time: "<<modification_time<<endl;

}
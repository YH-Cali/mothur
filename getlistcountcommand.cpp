/*
 *  getlistcountcommand.cpp
 *  Mothur
 *
 *  Created by westcott on 10/12/09.
 *  Copyright 2009 Schloss Lab. All rights reserved.
 *
 */

#include "getlistcountcommand.h"

//**********************************************************************************************************************
GetListCountCommand::GetListCountCommand(string option)  {
	try {
		globaldata = GlobalData::getInstance();
		abort = false;
		allLines = 1;
		labels.clear();
				
		//allow user to run help
		if(option == "help") { help(); abort = true; }
		
		else {
			//valid paramters for this command
			string AlignArray[] =  {"list","label","outputdir","inputdir"};
			vector<string> myArray (AlignArray, AlignArray+(sizeof(AlignArray)/sizeof(string)));
			
			OptionParser parser(option);
			map<string, string> parameters = parser.getParameters();
			
			ValidParameters validParameter;
			map<string, string>::iterator it;
			
			//check to make sure all parameters are valid for command
			for (it = parameters.begin(); it != parameters.end(); it++) { 
				if (validParameter.isValidParameter(it->first, myArray, it->second) != true) {  abort = true;  }
			}
			
			string ranRead = globaldata->getListFile();
			
			//if the user changes the input directory command factory will send this info to us in the output parameter 
			string inputDir = validParameter.validFile(parameters, "inputdir", false);		
			if (inputDir == "not found"){	inputDir = "";		}
			else {
				string path;
				it = parameters.find("list");
				//user has given a template file
				if(it != parameters.end()){ 
					path = hasPath(it->second);
					//if the user has not given a path then, add inputdir. else leave path alone.
					if (path == "") {	parameters["list"] = inputDir + it->second;		}
				}
			}

			//if the user changes the output directory command factory will send this info to us in the output parameter 
			outputDir = validParameter.validFile(parameters, "outputdir", false);		if (outputDir == "not found"){	outputDir = "";		}
			
			//check for required parameters
			listfile = validParameter.validFile(parameters, "list", true);
			if ((listfile == "not found") && (globaldata->getListFile() == ""))  { m->mothurOut("You must read a listfile before running the get.listcount command.");  m->mothurOutEndLine(); abort = true; }
			else if ((listfile == "not found") && (globaldata->getListFile() != "")) { listfile = globaldata->getListFile(); }
			else if (listfile == "not open") { abort = true; }	
			else { globaldata->setListFile(listfile); }
		
			//check for optional parameter and set defaults
			// ...at some point should added some additional type checking...
			
			label = validParameter.validFile(parameters, "label", false);			
			if (label == "not found") { label = ""; }
			else { 
				if(label != "all") {  splitAtDash(label, labels);  allLines = 0;  }
				else { allLines = 1;  }
			}
			
			//if the user has not specified any labels use the ones from read.otu
			if ((label == "") && (ranRead != "")) {  
				allLines = globaldata->allLines; 
				labels = globaldata->labels; 
			}
		}
	}
	catch(exception& e) {
		m->errorOut(e, "GetListCountCommand", "GetListCountCommand");
		exit(1);
	}
}
//**********************************************************************************************************************

void GetListCountCommand::help(){
	try {
		m->mothurOut("The get.listcount command can only be executed after a successful read.otu command of a listfile or providing a list file using the list parameter.\n");
		m->mothurOut("The get.listcount command parameters are list and label.  No parameters are required.\n");
		m->mothurOut("The label parameter allows you to select what distance levels you would like a output files created for, and are separated by dashes.\n");
		m->mothurOut("The get.listcount command should be in the following format: get.listcount(list=yourlistFile, label=yourLabels).\n");
		m->mothurOut("Example get.listcount(list=amazon.fn.list, label=0.10).\n");
		m->mothurOut("The default value for label is all lines in your inputfile.\n");
		m->mothurOut("The get.listcount command outputs a .otu file for each distance you specify listing the bin number and the names of the sequences in that bin.\n");
		m->mothurOut("Note: No spaces between parameter labels (i.e. list), '=' and parameters (i.e.yourListFile).\n\n");
	}
	catch(exception& e) {
		m->errorOut(e, "GetListCountCommand", "help");
		exit(1);
	}
}

//**********************************************************************************************************************

GetListCountCommand::~GetListCountCommand(){}

//**********************************************************************************************************************

int GetListCountCommand::execute(){
	try {
		if (abort == true) {	return 0;	}

		globaldata->setFormat("list");
		
		//read list file
		read = new ReadOTUFile(listfile);	
		read->read(&*globaldata); 
		
		input = globaldata->ginput;
		list = globaldata->gListVector;
		string lastLabel = list->getLabel();

		//if the users enters label "0.06" and there is no "0.06" in their file use the next lowest label.
		set<string> processedLabels;
		set<string> userLabels = labels;
		
		if (m->control_pressed) { 
			delete read;
			delete input;
			delete list;
			globaldata->gListVector = NULL;  
			for (int i = 0; i < outputNames.size(); i++) {	remove(outputNames[i].c_str());	}
			return 0; 
		}
		
		while((list != NULL) && ((allLines == 1) || (userLabels.size() != 0))) {
			
			if(allLines == 1 || labels.count(list->getLabel()) == 1){
			
				process(list);
				
				if (m->control_pressed) { 
					delete read;
					delete input;
					delete list;
					globaldata->gListVector = NULL;  
					for (int i = 0; i < outputNames.size(); i++) {	remove(outputNames[i].c_str());	}
					return 0; 
				}
							
				processedLabels.insert(list->getLabel());
				userLabels.erase(list->getLabel());
			}
			
			if ((anyLabelsToProcess(list->getLabel(), userLabels, "") == true) && (processedLabels.count(lastLabel) != 1)) {
				string saveLabel = list->getLabel();
				
				delete list;
				list = input->getListVector(lastLabel);
				
				process(list);
				
				if (m->control_pressed) { 
					delete read;
					delete input;
					delete list;
					globaldata->gListVector = NULL;  
					for (int i = 0; i < outputNames.size(); i++) {	remove(outputNames[i].c_str());	}
					return 0; 
				}
													
				processedLabels.insert(list->getLabel());
				userLabels.erase(list->getLabel());
				
				//restore real lastlabel to save below
				list->setLabel(saveLabel);
			}
			
			lastLabel = list->getLabel();			
			
			delete list;
			list = input->getListVector();
		}
		
		
		//output error messages about any remaining user labels
		set<string>::iterator it;
		bool needToRun = false;
		for (it = userLabels.begin(); it != userLabels.end(); it++) {  
			m->mothurOut("Your file does not include the label " + *it); 
			if (processedLabels.count(lastLabel) != 1) {
				m->mothurOut(". I will use " + lastLabel + "."); m->mothurOutEndLine();
				needToRun = true;
			}else {
				m->mothurOut(". Please refer to " + lastLabel + ".");  m->mothurOutEndLine();
			}
		}
		
		//run last label if you need to
		if (needToRun == true)  {
			if (list != NULL) {		delete list;	}
			list = input->getListVector(lastLabel);
				
			process(list);	
			
			if (m->control_pressed) { 
					delete read;
					delete input;
					delete list;
					globaldata->gListVector = NULL;  
					for (int i = 0; i < outputNames.size(); i++) {	remove(outputNames[i].c_str());	}
					return 0; 
			}
			
			delete list;  
		}
		
		delete read;
		delete input;
		globaldata->gListVector = NULL;
		
		m->mothurOutEndLine();
		m->mothurOut("Output File Names: "); m->mothurOutEndLine();
		for (int i = 0; i < outputNames.size(); i++) {	m->mothurOut(outputNames[i]); m->mothurOutEndLine();	}
		m->mothurOutEndLine();
		
		return 0;
	}
	catch(exception& e) {
		m->errorOut(e, "GetListCountCommand", "execute");
		exit(1);
	}
}

//**********************************************************************************************************************
//return 1 if error, 0 otherwise
void GetListCountCommand::process(ListVector* list) {
	try {
		string binnames, name, sequence;
		if (outputDir == "") { outputDir += hasPath(listfile); }
		string outputFileName = outputDir + getRootName(getSimpleName(listfile)) + list->getLabel() + ".otu";
		openOutputFile(outputFileName, out);
		outputNames.push_back(outputFileName);
		
		m->mothurOut(list->getLabel()); m->mothurOutEndLine();
		
		//for each bin in the list vector
		for (int i = 0; i < list->getNumBins(); i++) {
			if (m->control_pressed) { break; }
			
			binnames = list->get(i);
			out << i+1 << '\t' << binnames << endl;
		}
		
		out.close();
	}
	catch(exception& e) {
		m->errorOut(e, "GetListCountCommand", "process");
		exit(1);
	}
}
//**********************************************************************************************************************


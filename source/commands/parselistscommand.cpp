/*
 *  parselistcommand.cpp
 *  Mothur
 *
 *  Created by westcott on 2/24/10.
 *  Copyright 2010 Schloss Lab. All rights reserved.
 *
 */

#include "parselistscommand.h"

//**********************************************************************************************************************
vector<string> ParseListCommand::setParameters(){	
	try {
		CommandParameter plist("list", "InputTypes", "", "", "none", "none", "none","list",false,true,true); parameters.push_back(plist);
        CommandParameter pcount("count", "InputTypes", "", "", "CountGroup", "CountGroup", "none","",false,false,true); parameters.push_back(pcount);
		CommandParameter pgroup("group", "InputTypes", "", "", "CountGroup", "CountGroup", "none","",false,false,true); parameters.push_back(pgroup);
		CommandParameter plabel("label", "String", "", "", "", "", "","",false,false); parameters.push_back(plabel);
		CommandParameter pseed("seed", "Number", "", "0", "", "", "","",false,false); parameters.push_back(pseed);
        CommandParameter pinputdir("inputdir", "String", "", "", "", "", "","",false,false); parameters.push_back(pinputdir);
		CommandParameter poutputdir("outputdir", "String", "", "", "", "", "","",false,false); parameters.push_back(poutputdir);
		
		vector<string> myArray;
		for (int i = 0; i < parameters.size(); i++) {	myArray.push_back(parameters[i].name);		}
		return myArray;
	}
	catch(exception& e) {
		m->errorOut(e, "ParseListCommand", "setParameters");
		exit(1);
	}
}
//**********************************************************************************************************************
string ParseListCommand::getHelpString(){	
	try {
		string helpString = "";
		helpString += "The parse.list command reads a list and group or count file and generates a list file for each group in the group or count file. \n";
		helpString += "The parse.list command parameters are list, group, count and label.\n";
		helpString += "The list and group or count parameters are required.\n";
        helpString += "If a count file is provided, mothur assumes the list file contains only unique names.\n";
        helpString += "If a group file is provided, mothur assumes the list file contains all names.\n";
		helpString += "The label parameter is used to read specific labels in your input you want to use.\n";
		helpString += "The parse.list command should be used in the following format: parse.list(list=yourListFile, group=yourGroupFile, label=yourLabels).\n";
		helpString += "Example: parse.list(list=abrecovery.fn.list, group=abrecovery.groups, label=0.03).\n";
		return helpString;
	}
	catch(exception& e) {
		m->errorOut(e, "ParseListCommand", "getHelpString");
		exit(1);
	}
}
//**********************************************************************************************************************
string ParseListCommand::getOutputPattern(string type) {
    try {
        string pattern = "";
        
        if (type == "list") {  pattern = "[filename],[group],[distance],list"; } 
        else { m->mothurOut("[ERROR]: No definition for type " + type + " output pattern.\n"); m->setControl_pressed(true);  }
        
        return pattern;
    }
    catch(exception& e) {
        m->errorOut(e, "ParseListCommand", "getOutputPattern");
        exit(1);
    }
}
//**********************************************************************************************************************
ParseListCommand::ParseListCommand(){	
	try {
		abort = true; calledHelp = true; 
		setParameters();
		vector<string> tempOutNames;
		outputTypes["list"] = tempOutNames;
	}
	catch(exception& e) {
		m->errorOut(e, "ParseListCommand", "ParseListCommand");
		exit(1);
	}
}
//**********************************************************************************************************************
ParseListCommand::ParseListCommand(string option)  {
	try {
		abort = false; calledHelp = false;   
		allLines = true;
			
		//allow user to run help
		if(option == "help") { help(); abort = true; calledHelp = true; }
		else if(option == "citation") { citation(); abort = true; calledHelp = true;}
		
		else {
			vector<string> myArray = setParameters();
			
			OptionParser parser(option);
			map<string, string> parameters = parser.getParameters();
			
			ValidParameters validParameter;
			map<string, string>::iterator it;
		
			//check to make sure all parameters are valid for command
			for (it = parameters.begin(); it != parameters.end(); it++) { 
				if (!validParameter.isValidParameter(it->first, myArray, it->second)) {  abort = true;  }
			}
			
			//initialize outputTypes
			vector<string> tempOutNames;
			outputTypes["list"] = tempOutNames;			
												
			//if the user changes the input directory command factory will send this info to us in the output parameter 
			string inputDir = validParameter.valid(parameters, "inputdir");		
			if (inputDir == "not found"){	inputDir = "";		}
			else {
				string path;
				it = parameters.find("list");
				//user has given a template file
				if(it != parameters.end()){ 
					path = util.hasPath(it->second);
					//if the user has not given a path then, add inputdir. else leave path alone.
					if (path == "") {	parameters["list"] = inputDir + it->second;		}
				}
				
				it = parameters.find("group");
				//user has given a template file
				if(it != parameters.end()){ 
					path = util.hasPath(it->second);
					//if the user has not given a path then, add inputdir. else leave path alone.
					if (path == "") {	parameters["group"] = inputDir + it->second;		}
				}
                
                it = parameters.find("count");
				//user has given a template file
				if(it != parameters.end()){ 
					path = util.hasPath(it->second);
					//if the user has not given a path then, add inputdir. else leave path alone.
					if (path == "") {	parameters["count"] = inputDir + it->second;		}
				}
			}

			
			

			//check for required parameters
			listfile = validParameter.validFile(parameters, "list");
			if (listfile == "not open") { abort = true; }
			else if (listfile == "not found") { 
				listfile = current->getListFile(); 
				if (listfile != "") {  m->mothurOut("Using " + listfile + " as input file for the list parameter."); m->mothurOutEndLine(); }
				else { 
					m->mothurOut("No valid current list file. You must provide a list file."); m->mothurOutEndLine(); 
					abort = true;
						
				}
			}else { current->setListFile(listfile); }	
            
            //if the user changes the output directory command factory will send this info to us in the output parameter 
			outputDir = validParameter.valid(parameters, "outputdir");		if (outputDir == "not found"){	outputDir = util.hasPath(listfile);	}
			
            groupfile = validParameter.validFile(parameters, "group");
			if (groupfile == "not found") { groupfile =  "";   groupMap = NULL; }
			else if (groupfile == "not open") { abort = true; groupfile =  ""; groupMap = NULL; }	
			else {   
                current->setGroupFile(groupfile);
				groupMap = new GroupMap(groupfile);
				
				int error = groupMap->readMap();
				if (error == 1) { abort = true; }
            }
            
            countfile = validParameter.validFile(parameters, "count");
			if (countfile == "not found") { countfile =  "";   }
			else if (countfile == "not open") { abort = true; countfile =  ""; }	
			else {   
                current->setCountFile(countfile); 
                ct.readTable(countfile, true, false);
                if (!ct.hasGroupInfo()) { 
                    abort = true;
                    m->mothurOut("[ERROR]: The parse.list command requires group info to be present in your countfile, quitting."); m->mothurOutEndLine();
                }
                    
            }
            
            if ((groupfile != "") && (countfile != "")) {
                m->mothurOut("[ERROR]: you may only use one of the following: group or count."); m->mothurOutEndLine(); abort=true;
            }else if ((groupfile == "") && (countfile == "")) {
                m->mothurOut("[ERROR]: you must provide one of the following: group or count."); m->mothurOutEndLine(); abort=true;
            }
			
			//check for optional parameter and set defaults
			// ...at some point should added some additional type checking...
			label = validParameter.valid(parameters, "label");			
			if (label == "not found") { label = "";  allLines = true; }
			else { 
				if(label != "all") {  util.splitAtDash(label, labels);  allLines = false;  }
				else { allLines = true;  }
			}
		}

	}
	catch(exception& e) {
		m->errorOut(e, "ParseListCommand", "ParseListCommand");
		exit(1);
	}
}
//**********************************************************************************************************************
int ParseListCommand::execute(){
	try {
	
		if (abort) { if (calledHelp) { return 0; }  return 2;	}
		
		//if the users enters label "0.06" and there is no "0.06" in their file use the next lowest label.
		set<string> processedLabels;
		set<string> userLabels = labels;	
	
		InputData input(listfile, "list", nullVector);
		list = input.getListVector();
		string lastLabel = list->getLabel();
		
		if (m->getControl_pressed()) { 
			delete list; if (groupfile != "") { delete groupMap; }
			for (int i = 0; i < outputNames.size(); i++) {	util.mothurRemove(outputNames[i]); } outputTypes.clear(); return 0;
		}
		
		while((list != NULL) && ((allLines == 1) || (userLabels.size() != 0))) {
		
			if (m->getControl_pressed()) { 
				delete list; if (groupfile != "") { delete groupMap; }
				for (int i = 0; i < outputNames.size(); i++) {	util.mothurRemove(outputNames[i]); } outputTypes.clear();
				return 0;
			}
			
			if(allLines == 1 || labels.count(list->getLabel()) == 1){
					
					m->mothurOut(list->getLabel()); m->mothurOutEndLine();
					parse(list);
										
					processedLabels.insert(list->getLabel());
					userLabels.erase(list->getLabel());
			}
			
			if ((util.anyLabelsToProcess(list->getLabel(), userLabels, "") ) && (processedLabels.count(lastLabel) != 1)) {
					string saveLabel = list->getLabel();
					
					list = input.getListVector(lastLabel); //get new list vector to process
					
					m->mothurOut(list->getLabel()); m->mothurOutEndLine();
					parse(list);
					
					processedLabels.insert(list->getLabel());
					userLabels.erase(list->getLabel());
					
					//restore real lastlabel to save below
					list->setLabel(saveLabel);
			}
			
		
			lastLabel = list->getLabel();
				
			delete list;
			list = input.getListVector(); //get new list vector to process
		}
		
		if (m->getControl_pressed()) { 
			if (groupfile != "") { delete groupMap; }
			for (int i = 0; i < outputNames.size(); i++) {	util.mothurRemove(outputNames[i]); } outputTypes.clear();
			return 0;
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
				m->mothurOut(". Please refer to " + lastLabel + "."); m->mothurOutEndLine();
			}

		}
		
		if (m->getControl_pressed()) { 
			if (groupfile != "") { delete groupMap; }
			for (int i = 0; i < outputNames.size(); i++) {	util.mothurRemove(outputNames[i]); } outputTypes.clear();
			return 0;
		}
		
		//run last label if you need to
		if (needToRun )  {
			if (list != NULL) {	delete list;	}
			list = input.getListVector(lastLabel); //get new list vector to process
			
			m->mothurOut(list->getLabel()); m->mothurOutEndLine();
			parse(list);		
			
			delete list;
		}
		
		if (groupfile != "") { delete groupMap; }
		
		if (m->getControl_pressed()) { 
			for (int i = 0; i < outputNames.size(); i++) {	util.mothurRemove(outputNames[i]); } outputTypes.clear();
			return 0;
		}
		
		//set fasta file as new current fastafile
		string currentName = "";
		itTypes = outputTypes.find("list");
		if (itTypes != outputTypes.end()) {
			if ((itTypes->second).size() != 0) { currentName = (itTypes->second)[0]; current->setListFile(currentName); }
		}
			
		m->mothurOut("\nOutput File Names: \n"); 
		for (int i = 0; i < outputNames.size(); i++) {	m->mothurOut(outputNames[i] +"\n"); 	} m->mothurOutEndLine();
		
		return 0;
	}
	catch(exception& e) {
		m->errorOut(e, "ParseListCommand", "execute");
		exit(1);
	}
}
/**********************************************************************************************************************/
int ParseListCommand::parse(ListVector* thisList) {
	try {

		//fill filehandles with neccessary ofstreams
		vector<string> gGroups;
        if (groupfile != "") { gGroups = groupMap->getNamesOfGroups(); }
        else { gGroups = ct.getNamesOfGroups(); }
        
        map<string, string> groupVector;
        map<string, string> groupLabels;
        map<string, string>::iterator itGroup;
        map<string, int> groupNumBins;
        
        map<string, string> files;
        
        //set fileroot
        map<string, string> variables;
        variables["[filename]"] = outputDir + util.getRootName(util.getSimpleName(listfile));
        variables["[distance]"] = thisList->getLabel();
        
		for (int i=0; i<gGroups.size(); i++) {
            variables["[group]"] = gGroups[i];
			string filename = getOutputFileName("list",variables);
			ofstream temp;
            util.openOutputFile(filename, temp); temp.close();
            files[gGroups[i]] = filename;
            outputNames.push_back(filename); outputTypes["list"].push_back(filename);
            groupNumBins[gGroups[i]] = 0;
            groupVector[gGroups[i]] = "";
            groupLabels[gGroups[i]] = "label\tnumOtus";
		}

		vector<string> binLabels = thisList->getLabels();
		for (int i = 0; i < thisList->getNumBins(); i++) {
			if (m->getControl_pressed()) { break; }
			
			map<string, string> groupBins;
			string bin = list->get(i); 
			
			vector<string> names;
			util.splitAtComma(bin, names);  //parses bin into individual sequence names
			
			//parse bin into list of sequences in each group
			for (int j = 0; j < names.size(); j++) {
                if (groupfile != "") {
                    string group = groupMap->getGroup(names[j]);
				
                    if (group == "not found") { m->mothurOut(names[j] + " is not in your groupfile. please correct."); m->mothurOutEndLine(); exit(1); }
				
                    itGroup = groupBins.find(group);
                    if(itGroup == groupBins.end()) {
                        groupBins[group] = names[j];  //add first name
                        groupNumBins[group]++;
                    }else{ //add another name
                        groupBins[group] = groupBins[group] + "," + names[j];
                    }
                }else{
                    vector<string> thisSeqsGroups = ct.getGroups(names[j]);
                    
                    for (int k = 0; k < thisSeqsGroups.size(); k++) {
                        string group = thisSeqsGroups[k];
                        itGroup = groupBins.find(group);
                        if(itGroup == groupBins.end()) {
                            groupBins[group] = names[j];  //add first name
                            groupNumBins[group]++;
                        }else{ //add another name
                            groupBins[group] = groupBins[group] + "," + names[j];
                        }

                    }
                }
			}
			
			//print parsed bin info to files
			for (itGroup = groupBins.begin(); itGroup != groupBins.end(); itGroup++) {
				groupVector[itGroup->first] +=  '\t' + itGroup->second;
                groupLabels[itGroup->first] +=  '\t' + binLabels[i];
			}
		
		}
        
		//end list vector
		for (map<string, string>::iterator it3 = files.begin(); it3 != files.end(); it3++) {
            ofstream out;
            string filename = it3->second;
            util.openOutputFileAppend(filename, out);
            out << groupLabels[it3->first] << endl;
			out << thisList->getLabel() << '\t' << groupNumBins[it3->first] << groupVector[it3->first] << endl;  // label numBins  listvector for that group
            out.close();
		}
		
		return 0;

	}
	catch(exception& e) {
		m->errorOut(e, "ParseListCommand", "parse");
		exit(1);
	}
}

/**********************************************************************************************************************/



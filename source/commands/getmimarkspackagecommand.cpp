//
//  getmimarkspackagecommand.cpp
//  Mothur
//
//  Created by Sarah Westcott on 3/25/14.
//  Copyright (c) 2014 Schloss Lab. All rights reserved.
//

#include "getmimarkspackagecommand.h"
#include "groupmap.h"


//**********************************************************************************************************************
vector<string> GetMIMarksPackageCommand::setParameters(){
	try {
        //files that have dependancies
        CommandParameter pgroup("group", "InputTypes", "", "", "groupOligos", "none", "none","",false,false); parameters.push_back(pgroup);
        CommandParameter pfile("file", "InputTypes", "", "", "groupOligos", "none", "none","",false,false); parameters.push_back(pfile);
        CommandParameter poligos("oligos", "InputTypes", "", "", "groupOligos", "none", "none","",false,false); parameters.push_back(poligos);
        CommandParameter ppackage("package", "Multiple", "air-host_associated-human_associated-human_gut-human_oral-human_skin-human_vaginal-microbial-miscellaneous-plant_associated-sediment-soil-wastewater-water", "miscellaneous", "", "", "","",false,false,true); parameters.push_back(ppackage);
        CommandParameter prequiredonly("requiredonly", "Boolean", "", "F", "", "", "","",false,false, true); parameters.push_back(prequiredonly);
  		CommandParameter pinputdir("inputdir", "String", "", "", "", "", "","",false,false); parameters.push_back(pinputdir);
		CommandParameter poutputdir("outputdir", "String", "", "", "", "", "","",false,false); parameters.push_back(poutputdir);
		
		vector<string> myArray;
		for (int i = 0; i < parameters.size(); i++) {	myArray.push_back(parameters[i].name);		}
		return myArray;
	}
	catch(exception& e) {
		m->errorOut(e, "GetMIMarksPackageCommand", "setParameters");
		exit(1);
	}
}
//**********************************************************************************************************************
string GetMIMarksPackageCommand::getHelpString(){
	try {
		string helpString = "";
		helpString += "The get.mimarkspackage command creates a mimarks package form with your groups. The required fields are flagged with * characters. \n";
        helpString += "Further documentation on the different packages and required formats can be found here, http://www.mothur.org/wiki/MIMarks_Data_Packages.\n";
		helpString += "The get.mimarkspackage command parameters are: oligos, group, package and requiredonly. oligos or group is required.\n";
		helpString += "The oligos parameter is used to provide your oligos file so mothur can extract your group names.\n";
        helpString += "The group parameter is used to provide your group file so mothur can extract your group names.\n";
        helpString += "The package parameter is used to select the mimarks package you would like to use. The choices are: air, host_associated, human_associated, human_gut, human_oral, human_skin, human_vaginal, microbial, miscellaneous, plant_associated, sediment, soil, wastewater or waterc. Default=miscellaneous.\n";
        helpString += "The requiredonly parameter is used to indicate you only want the required mimarks feilds printed. Default=F.\n";
		helpString += "The get.mimarkspackage command should be in the following format: get.mimarkspackage(oligos=yourOligosFile, package=yourPackage)\n";
		helpString += "get.mimarkspackage(oligos=GQY1XT001.oligos, package=human_gut)\n";
		return helpString;
	}
	catch(exception& e) {
		m->errorOut(e, "GetMIMarksPackageCommand", "getHelpString");
		exit(1);
	}
}
//**********************************************************************************************************************
string GetMIMarksPackageCommand::getOutputPattern(string type) {
    try {
        string pattern = "";
        
        if (type == "tsv") {  pattern = "[filename],tsv"; }
        else { m->mothurOut("[ERROR]: No definition for type " + type + " output pattern.\n"); m->control_pressed = true;  }
        
        return pattern;
    }
    catch(exception& e) {
        m->errorOut(e, "GetMIMarksPackageCommand", "getOutputPattern");
        exit(1);
    }
}
//**********************************************************************************************************************
GetMIMarksPackageCommand::GetMIMarksPackageCommand(){
	try {
		abort = true; calledHelp = true;
		setParameters();
        vector<string> tempOutNames;
		outputTypes["tsv"] = tempOutNames;
	}
	catch(exception& e) {
		m->errorOut(e, "GetMIMarksPackageCommand", "GetMIMarksPackageCommand");
		exit(1);
	}
}
//**********************************************************************************************************************
GetMIMarksPackageCommand::GetMIMarksPackageCommand(string option)  {
	try {
        
        abort = false; calledHelp = false; fileOption = 0;
		
		//allow user to run help
		if(option == "help") { help(); abort = true; calledHelp = true; }
		else if(option == "citation") { citation(); abort = true; calledHelp = true;}
		
		else {
			//valid paramters for this command
			vector<string> myArray = setParameters();
			
			OptionParser parser(option);
			map<string,string> parameters = parser.getParameters();
			
			ValidParameters validParameter;
			map<string,string>::iterator it;
			//check to make sure all parameters are valid for command
			for (it = parameters.begin(); it != parameters.end(); it++) {
				if (validParameter.isValidParameter(it->first, myArray, it->second) != true) {  abort = true;  }
			}
			
            vector<string> tempOutNames;
			outputTypes["tsv"] = tempOutNames;
            
			//if the user changes the input directory command factory will send this info to us in the output parameter
			inputDir = validParameter.validFile(parameters, "inputdir", false);
			if (inputDir == "not found"){	inputDir = "";		}
			else {
                
				string path;
				it = parameters.find("oligos");
				//user has given a template file
				if(it != parameters.end()){
					path = m->hasPath(it->second);
					//if the user has not given a path then, add inputdir. else leave path alone.
					if (path == "") {	parameters["oligos"] = inputDir + it->second;		}
				}
				
				it = parameters.find("group");
				//user has given a template file
				if(it != parameters.end()){
					path = m->hasPath(it->second);
					//if the user has not given a path then, add inputdir. else leave path alone.
					if (path == "") {	parameters["group"] = inputDir + it->second;		}
				}
                
                it = parameters.find("file");
				//user has given a template file
				if(it != parameters.end()){
					path = m->hasPath(it->second);
					//if the user has not given a path then, add inputdir. else leave path alone.
                    if (path == "") {	parameters["file"] = inputDir + it->second;		}
                }
				
            }
            
			groupfile = validParameter.validFile(parameters, "group", true);
			if (groupfile == "not open") {  groupfile = "";  abort = true; }
			else if (groupfile == "not found") { groupfile = ""; }
            else {  m->setGroupFile(groupfile); inputfile = groupfile; }
            
            oligosfile = validParameter.validFile(parameters, "oligos", true);
            if (oligosfile == "not found")      {	oligosfile = "";	setOligosParameter = false; }
            else if(oligosfile == "not open")	{	abort = true;		}
            else {	m->setOligosFile(oligosfile); inputfile = oligosfile; setOligosParameter = true; }

            file = validParameter.validFile(parameters, "file", true);
			if (file == "not open") {  file = "";  abort = true; }
			else if (file == "not found") { file = ""; }
            else {  inputfile = file;  fileOption = findFileOption();  }
            
            
            if ((groupfile == "") && (oligosfile == "") && (file == "")) {
                oligosfile = m->getOligosFile();
                if (oligosfile != "") { inputfile = oligosfile;  m->mothurOut("Using " + oligosfile + " as input file for the oligos parameter."); m->mothurOutEndLine(); }
                else {
                    groupfile = m->getGroupFile();
                    if (groupfile != "") { inputfile = groupfile;  m->mothurOut("Using " + groupfile + " as input file for the group parameter."); m->mothurOutEndLine(); }
                    else {
                        m->mothurOut("[ERROR]: You must provide file, groupfile or oligos file for the get.mimarkspackage command."); m->mothurOutEndLine(); abort = true;
                    }
                }
            }
            
            package = validParameter.validFile(parameters, "package", false);         if (package == "not found") { package = "Miscellaneous"; }
            
            if (package.length() != 0) { package[0] = toupper(package[0]); }
            for (int i = 1; i < package.length(); i++) { package[i] = tolower(package[i]); }
            
            if ((package == "Air") || (package == "Host_associated") || (package == "Human_associated") || (package == "Human_gut") || (package == "Human_oral") || (package == "Human_skin") || (package == "Human_vaginal") || (package == "Microbial") || (package == "Miscellaneous")  || (package == "Plant_associated") || (package == "Sediment") || (package == "Soil") || (package == "Wastewater") || (package == "Water") ) {}
            else {
                m->mothurOut("[ERROR]: " + package + " is not a valid package selection. Choices are: air, host_associated, human_associated, human_gut, human_oral, human_skin, human_vaginal, microbial, miscellaneous, plant_associated, sediment, soil, wastewater or water. Aborting.\n."); abort = true;
            }
            
            
            
            string temp;
			temp = validParameter.validFile(parameters, "requiredonly", false);	if(temp == "not found"){	temp = "F";	}
			requiredonly = m->isTrue(temp);
		}
		
	}
	catch(exception& e) {
		m->errorOut(e, "GetMIMarksPackageCommand", "GetMIMarksPackageCommand");
		exit(1);
	}
}
//**********************************************************************************************************************

int GetMIMarksPackageCommand::execute(){
	try {
		
		if (abort == true) { if (calledHelp) { return 0; }  return 2;	}
        
        if ((oligosfile != "") && (file != "")) { Oligos oligos(oligosfile); createGroupNames(oligos);  }
        else if (file != "")  { readFile();     }
        else if (oligosfile != "") { Oligos oligos(oligosfile); createGroupNames(oligos);  } //createGroupNames fills in group names
        else {  GroupMap groupmap(groupfile); groupmap.readMap();
            vector<string> tempGroups = groupmap.getNamesOfGroups();
            for (int i = 0; i < tempGroups.size(); i++) { Groups.insert(tempGroups[i]); }
        }
        
        if (outputDir == "") { outputDir += m->hasPath(inputfile); }
        map<string, string> variables;
		variables["[filename]"] = outputDir + m->getRootName(m->getSimpleName(inputfile));
		string outputFileName = getOutputFileName("tsv", variables);
		
        ofstream out;
		m->openOutputFile(outputFileName, out);
		outputNames.push_back(outputFileName); outputTypes["tsv"].push_back(outputFileName);
        
        out << "#This is a tab-delimited file. Additional Documentation can be found at http://www.mothur.org/wiki/MIMarks_Data_Packages." << endl;
        out << "#Please fill all the required fields indicated with '*'" << endl;
        out << "#Unknown or inapplicable fields can be assigned NA value." << endl;
        out << "#You may add extra custom fields to this template. Make sure all the fields are separated by tabs." << endl;
        out << "#You may remove any fields not required (marked with '*'). Make sure all the fields are separated by tabs." << endl;
        out << "#You can edit this template using Microsoft Excel or any other editor. But while saving the file please make sure to save them as 'TAB-DELIMITED' TEXT FILE." << endl;
        
        if (package == "Air") {
            out << "#MIMARKS.survey.air.4.0" << endl;
            if (requiredonly) {
                out << "#{sample name}	{description of sample}	{sample title}	{Date of sampling, in \"DD-Mmm-YYYY\", \"Mmm-YYYY\" or \"YYYY\" format (eg., 30-Oct-1990, Oct-1990 or 1990) or ISO 8601 standard \"YYYY-mm-dd\", \"YYYY-mm\" or \"YYYY-mm-ddThh:mm:ss\" (eg., 1990-10-30, 1990-10 or 1990-10-30T14:41:36)}	{descriptor of the broad ecological context of a sample. Examples include: desert, taiga, deciduous woodland, or coral reef. EnvO (v 2013-06-14) terms can be found via the link: www.environmentontology.org/Browse-EnvO}	{descriptor of the local environment. Examples include: harbor, cliff, or lake. EnvO (v 2013-06-14) terms can be found via the link: www.environmentontology.org/Browse-EnvO}	{material that was displaced by the sample, or material in which a sample was embedded, prior to the sampling event. Examples include: air, soil, or water. EnvO (v 2013-06-14) terms can be found via the link: www.environmentontology.org/Browse-EnvO}	{Geographical origin of the sample; use the appropriate name from this list http://www.insdc.org/documents/country-qualifier-vocabulary. Use a colon to separate the country or ocean from more detailed information about the location, eg \"Canada: Vancouver\" or \"Germany: halfway down Zugspitze, Alps\"}	{The geographical coordinates of the location where the sample was collected. Specify as degrees latitude and longitude in format \"d[d.dddd] N|S d[dd.dddd] W|E\", eg, 38.98 N 77.11 W}	{The altitude of the sample is the vertical distance between Earth's surface above Sea Level and the sampled position in the air.}" << endl;
                out << "#{text}	{text}	{text}	{{timestamp}}	{{term}}	{{term}}	{{term}}	{{term}:{term}:{text}}	{{float} {float}}	{{float} m}" << endl;
                out << "*sample_name	*description	*sample_title	*collection_date	*env_biome	*env_feature	*env_material	*geo_loc_name	*lat_lon	*altitude" << endl;
            }else {
                out << "#{sample name}	{description of sample}	{sample title}	{Date of sampling, in \"DD-Mmm-YYYY\", \"Mmm-YYYY\" or \"YYYY\" format (eg., 30-Oct-1990, Oct-1990 or 1990) or ISO 8601 standard \"YYYY-mm-dd\", \"YYYY-mm\" or \"YYYY-mm-ddThh:mm:ss\" (eg., 1990-10-30, 1990-10 or 1990-10-30T14:41:36)}	{descriptor of the broad ecological context of a sample. Examples include: desert, taiga, deciduous woodland, or coral reef. EnvO (v 2013-06-14) terms can be found via the link: www.environmentontology.org/Browse-EnvO}	{descriptor of the local environment. Examples include: harbor, cliff, or lake. EnvO (v 2013-06-14) terms can be found via the link: www.environmentontology.org/Browse-EnvO}	{material that was displaced by the sample, or material in which a sample was embedded, prior to the sampling event. Examples include: air, soil, or water. EnvO (v 2013-06-14) terms can be found via the link: www.environmentontology.org/Browse-EnvO}	{Geographical origin of the sample; use the appropriate name from this list http://www.insdc.org/documents/country-qualifier-vocabulary. Use a colon to separate the country or ocean from more detailed information about the location, eg \"Canada: Vancouver\" or \"Germany: halfway down Zugspitze, Alps\"}	{The geographical coordinates of the location where the sample was collected. Specify as degrees latitude and longitude in format \"d[d.dddd] N|S d[dd.dddd] W|E\", eg, 38.98 N 77.11 W}	{The altitude of the sample is the vertical distance between Earth's surface above Sea Level and the sampled position in the air.}		{force per unit area exerted against a surface by the weight of air above that surface}	{carbon dioxide (gas) amount or concentration at the time of sampling}	{carbon monoxide (gas) amount or concentration at the time of sampling}	{list of chemical compounds administered to the host or site where sampling occurred, and when (e.g. antibiotics, N fertilizer, air filter); can include multiple compounds. For Chemical Entities of Biological Interest ontology (CHEBI) (v1.72), please see http://bioportal.bioontology.org/visualize/44603}	{The elevation of the sampling site as measured by the vertical distance from mean sea level.}	{amount of water vapour in the air, at the time of sampling}	{methane (gas) amount or concentration at the time of sampling}	{any other measurement performed or parameter collected, that is not listed here}	{total count of any organism per gram or volume of sample,should include name of organism followed by count; can include multiple organism counts}	{oxygenation status of sample}	{oxygen (gas) amount or concentration at the time of sampling}	{type of perturbation, e.g. chemical administration, physical disturbance, etc., coupled with time that perturbation occurred; can include multiple perturbation types}	{pollutant types and, amount or concentrations measured at the time of sampling; can report multiple pollutants by entering numeric values preceded by name of pollutant}	{concentration of substances that remain suspended in the air, and comprise mixtures of organic and inorganic substances (PM10 and PM2.5); can report multiple PM's by entering numeric values preceded by name of PM}	{none}	{none}	{none}	{none}	{the amount of solar energy that arrives at a specific area of a surface during a specific time interval}	{temperature of the sample at time of sampling}	{ventilation rate of the system in the sampled premises}	{ventilation system used in the sampled premises}	{concentration of carbon-based chemicals that easily evaporate at room temperature; can report multiple volatile organic compounds by entering numeric values preceded by name of compound}	{wind direction is the direction from which a wind originates}	{speed of wind measured at the time of sampling}" << endl;
                out << "#{text}	{text}	{text}	{{timestamp}}	{{term}}	{{term}}	{{term}}	{{term}:{term}:{text}}	{{float} {float}}	{{float} m}		{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{term}; {timestamp}}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{text};{float} {unit}}	{{text};{float} {unit}}	{['', 'aerobic', 'anaerobic']}	{{float} {unit}}	{{text};{interval}}	{{text};{float} {unit}}	{{text};{float} {unit}}	{{none}}	{{none}}	{{none}}	{{none}}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{text}}	{{text};{float} {unit}}	{{text}}	{{float} {unit}}" << endl;
                out << "*sample_name	*description	*sample_title	*collection_date	*env_biome	*env_feature	*env_material	*geo_loc_name	*lat_lon	*altitude		barometric_press	carb_dioxide	carb_monoxide	chem_administration	elev	humidity	methane	misc_param	organism_count	oxy_stat_samp	oxygen	perturbation	pollutants	resp_part_matter	samp_salinity	samp_store_dur	samp_store_loc	samp_store_temp	solar_irradiance	temp	ventilation_rate	ventilation_type	volatile_org_comp	wind_direction	wind_speed" << endl;
            }
        }else if (package == "Host_associated") {
            out << "#MIMARKS.survey.host-associated.4.0" << endl;
            if (requiredonly) {
                out << "#{sample name}	{description of sample}	{sample title}	{Date of sampling, in \"DD-Mmm-YYYY\", \"Mmm-YYYY\" or \"YYYY\" format (eg., 30-Oct-1990, Oct-1990 or 1990) or ISO 8601 standard \"YYYY-mm-dd\", \"YYYY-mm\" or \"YYYY-mm-ddThh:mm:ss\" (eg., 1990-10-30, 1990-10 or 1990-10-30T14:41:36)}	{descriptor of the broad ecological context of a sample. Examples include: desert, taiga, deciduous woodland, or coral reef. EnvO (v 2013-06-14) terms can be found via the link: www.environmentontology.org/Browse-EnvO}	{descriptor of the local environment. Examples include: harbor, cliff, or lake. EnvO (v 2013-06-14) terms can be found via the link: www.environmentontology.org/Browse-EnvO}	{material that was displaced by the sample, or material in which a sample was embedded, prior to the sampling event. Examples include: air, soil, or water. EnvO (v 2013-06-14) terms can be found via the link: www.environmentontology.org/Browse-EnvO}	{Geographical origin of the sample; use the appropriate name from this list http://www.insdc.org/documents/country-qualifier-vocabulary. Use a colon to separate the country or ocean from more detailed information about the location, eg \"Canada: Vancouver\" or \"Germany: halfway down Zugspitze, Alps\"}	{The geographical coordinates of the location where the sample was collected. Specify as degrees latitude and longitude in format \"d[d.dddd] N|S d[dd.dddd] W|E\", eg, 38.98 N 77.11 W}" << endl;
                out << "#{text}	{text}	{text}	{{timestamp}}	{{term}}	{{term}}	{{term}}	{{term}:{term}:{text}}	{{float} {float}}" << endl;
                out << "*sample_name	*description	*sample_title	*collection_date	*env_biome	*env_feature	*env_material	*geo_loc_name	*lat_lon" << endl;
            }else {
                out << "#{sample name}	{description of sample}	{sample title}	{Date of sampling, in \"DD-Mmm-YYYY\", \"Mmm-YYYY\" or \"YYYY\" format (eg., 30-Oct-1990, Oct-1990 or 1990) or ISO 8601 standard \"YYYY-mm-dd\", \"YYYY-mm\" or \"YYYY-mm-ddThh:mm:ss\" (eg., 1990-10-30, 1990-10 or 1990-10-30T14:41:36)}	{descriptor of the broad ecological context of a sample. Examples include: desert, taiga, deciduous woodland, or coral reef. EnvO (v 2013-06-14) terms can be found via the link: www.environmentontology.org/Browse-EnvO}	{descriptor of the local environment. Examples include: harbor, cliff, or lake. EnvO (v 2013-06-14) terms can be found via the link: www.environmentontology.org/Browse-EnvO}	{material that was displaced by the sample, or material in which a sample was embedded, prior to the sampling event. Examples include: air, soil, or water. EnvO (v 2013-06-14) terms can be found via the link: www.environmentontology.org/Browse-EnvO}	{Geographical origin of the sample; use the appropriate name from this list http://www.insdc.org/documents/country-qualifier-vocabulary. Use a colon to separate the country or ocean from more detailed information about the location, eg \"Canada: Vancouver\" or \"Germany: halfway down Zugspitze, Alps\"}	{The geographical coordinates of the location where the sample was collected. Specify as degrees latitude and longitude in format \"d[d.dddd] N|S d[dd.dddd] W|E\", eg, 38.98 N 77.11 W}	{The altitude of the sample is the vertical distance between Earth's surface above Sea Level and the sampled position in the air.}	{list of chemical compounds administered to the host or site where sampling occurred, and when (e.g. antibiotics, N fertilizer, air filter); can include multiple compounds. For Chemical Entities of Biological Interest ontology (CHEBI) (v1.72), please see http://bioportal.bioontology.org/visualize/44603}	{Depth is defined as the vertical distance below surface, e.g. for sediment or soil samples depth is measured from sediment or soil surface, respectivly. Depth can be reported as an interval for subsurface samples.}	{The elevation of the sampling site as measured by the vertical distance from mean sea level.}	{whether or not subject is gravid, and if yes date due or date post-conception, specifying which is used}	{Age of host at the time of sampling}	{resting diastolic blood pressureof the host, measured as mm mercury}	{resting systolic blood pressure of the host, measured as mm mercury}	{original body habitat where the sample was obtained from}	{substance produced by the host, e.g. stool, mucus, where the sample was obtained from}	{core body temperature of the host when sample was collected}	{the color of host}	{type of diet depending on the sample for animals omnivore, herbivore etc., for humans high-fat, meditteranean etc.; can include multiple diet types}	{Name of relevant disease, e.g. Salmonella gastroenteritis. Controlled vocabulary, http://bioportal.bioontology.org/ontologies/1009 or http://www.ncbi.nlm.nih.gov/mesh}	{measurement of dry mass}	{none}	{none}	{literature reference giving growth conditions of the host}	{the height of subject}	{taxonomic information subspecies level}	{taxonomic rank information below subspecies level, such as variety, form, rank etc.}	{content of last meal and time since feeding; can include multiple values}	{the length of subject}	{description of host life stage}	{none}	{Gender or physical sex of the host}	{morphological shape of host}	{a unique identifier by which each subject can be referred to, de-identified, e.g. #131}	{the growth substrate of the host}	{NCBI taxonomy ID of the host, e.g. 9606}	{Type of tissue the initial sample was taken from. Controlled vocabulary, http://bioportal.bioontology.org/ontologies/1005)}	{total mass of the host at collection, the unit depends on host}	{any other measurement performed or parameter collected, that is not listed here}	{total count of any organism per gram or volume of sample,should include name of organism followed by count; can include multiple organism counts}	{oxygenation status of sample}	{type of perturbation, e.g. chemical administration, physical disturbance, etc., coupled with time that perturbation occurred; can include multiple perturbation types}	{none}	{none}	{none}	{none}	{temperature of the sample at time of sampling}" << endl;
                out << "#{text}	{text}	{text}	{{timestamp}}	{{term}}	{{term}}	{{term}}	{{term}:{term}:{text}}	{{float} {float}}	{{float} m}	{{term}; {timestamp}}	{{float} m}	{{float} {unit}}	{{boolean};{timestamp}}	{{none}}	{{float} {unit}}	{{float} {unit}}	{{term}}	{{text}}	{{float} {unit}}	{{text}}	{{text}}	{{none}}	{{float} {unit}}	{{none}}	{{none}}	{{PMID|DOI|URL}}	{{float} {unit}}	{{text}}	{{text}}	{{text};{period}}	{{float} {unit}}	{{text}}	{{none}}	{['', 'male', 'female', 'pooled male and female', 'neuter', 'hermaphrodite', 'not determined', 'missing', 'not applicable', 'not collected']}	{{text}}	{{text}}	{{text}}	{{integer}}	{{none}}	{{float} {unit}}	{{text};{float} {unit}}	{{text};{float} {unit}}	{['', 'aerobic', 'anaerobic']}	{{text};{interval}}	{{none}}	{{none}}	{{none}}	{{none}}	{{float} {unit}}" << endl;
                out << "*sample_name	*description	*sample_title	*collection_date	*env_biome	*env_feature	*env_material	*geo_loc_name	*lat_lon	altitude	chem_administration	depth	elev	gravidity	host_age	host_blood_press_diast	host_blood_press_syst	host_body_habitat	host_body_product	host_body_temp	host_color	host_diet	host_disease	host_dry_mass	host_family_relationship	host_genotype	host_growth_cond	host_height	host_infra_specific_name	host_infra_specific_rank	host_last_meal	host_length	host_life_stage	host_phenotype	host_sex	host_shape	host_subject_id	host_substrate	host_taxid	host_tissue_sampled	host_tot_mass	misc_param	organism_count	oxy_stat_samp	perturbation	samp_salinity	samp_store_dur	samp_store_loc	samp_store_temp	temp" << endl;
            }
        }else if (package == "Human_associated") {
            out << "#MIMARKS.survey.human-associated.4.0" << endl;
            if (requiredonly) {
                out << "#{sample name}	{description of sample}	{sample title}	{Date of sampling, in \"DD-Mmm-YYYY\", \"Mmm-YYYY\" or \"YYYY\" format (eg., 30-Oct-1990, Oct-1990 or 1990) or ISO 8601 standard \"YYYY-mm-dd\", \"YYYY-mm\" or \"YYYY-mm-ddThh:mm:ss\" (eg., 1990-10-30, 1990-10 or 1990-10-30T14:41:36)}	{descriptor of the broad ecological context of a sample. Examples include: desert, taiga, deciduous woodland, or coral reef. EnvO (v 2013-06-14) terms can be found via the link: www.environmentontology.org/Browse-EnvO}	{descriptor of the local environment. Examples include: harbor, cliff, or lake. EnvO (v 2013-06-14) terms can be found via the link: www.environmentontology.org/Browse-EnvO}	{material that was displaced by the sample, or material in which a sample was embedded, prior to the sampling event. Examples include: air, soil, or water. EnvO (v 2013-06-14) terms can be found via the link: www.environmentontology.org/Browse-EnvO}	{Geographical origin of the sample; use the appropriate name from this list http://www.insdc.org/documents/country-qualifier-vocabulary. Use a colon to separate the country or ocean from more detailed information about the location, eg \"Canada: Vancouver\" or \"Germany: halfway down Zugspitze, Alps\"}	{The geographical coordinates of the location where the sample was collected. Specify as degrees latitude and longitude in format \"d[d.dddd] N|S d[dd.dddd] W|E\", eg, 38.98 N 77.11 W}" << endl;
                out << "#{text}	{text}	{text}	{{timestamp}}	{{term}}	{{term}}	{{term}}	{{term}:{term}:{text}}	{{float} {float}}" << endl;
                out << "*sample_name	*description	*sample_title	*collection_date	*env_biome	*env_feature	*env_material	*geo_loc_name	*lat_lon" << endl;
            }else {
                out << "#{sample name}	{description of sample}	{sample title}	{Date of sampling, in \"DD-Mmm-YYYY\", \"Mmm-YYYY\" or \"YYYY\" format (eg., 30-Oct-1990, Oct-1990 or 1990) or ISO 8601 standard \"YYYY-mm-dd\", \"YYYY-mm\" or \"YYYY-mm-ddThh:mm:ss\" (eg., 1990-10-30, 1990-10 or 1990-10-30T14:41:36)}	{descriptor of the broad ecological context of a sample. Examples include: desert, taiga, deciduous woodland, or coral reef. EnvO (v 2013-06-14) terms can be found via the link: www.environmentontology.org/Browse-EnvO}	{descriptor of the local environment. Examples include: harbor, cliff, or lake. EnvO (v 2013-06-14) terms can be found via the link: www.environmentontology.org/Browse-EnvO}	{material that was displaced by the sample, or material in which a sample was embedded, prior to the sampling event. Examples include: air, soil, or water. EnvO (v 2013-06-14) terms can be found via the link: www.environmentontology.org/Browse-EnvO}	{Geographical origin of the sample; use the appropriate name from this list http://www.insdc.org/documents/country-qualifier-vocabulary. Use a colon to separate the country or ocean from more detailed information about the location, eg \"Canada: Vancouver\" or \"Germany: halfway down Zugspitze, Alps\"}	{The geographical coordinates of the location where the sample was collected. Specify as degrees latitude and longitude in format \"d[d.dddd] N|S d[dd.dddd] W|E\", eg, 38.98 N 77.11 W}	{specification of the color of the amniotic fluid sample}	{history of blood disorders; can include multiple disorders}	{list of chemical compounds administered to the host or site where sampling occurred, and when (e.g. antibiotics, N fertilizer, air filter); can include multiple compounds. For Chemical Entities of Biological Interest ontology (CHEBI) (v1.72), please see http://bioportal.bioontology.org/visualize/44603}	{specification of major diet changes in the last six months, if yes the change should be specified}	{any drug used by subject and the frequency of usage; can include multiple drugs used}	{ethnicity of the subject}	{specification of foetal health status, should also include abortion}	{specification of the gestation state}	{Age of host at the time of sampling}	{body mass index of the host, calculated as weight/(height)squared}	{substance produced by the host, e.g. stool, mucus, where the sample was obtained from}	{core body temperature of the host when sample was collected}	{type of diet depending on the sample for animals omnivore, herbivore etc., for humans high-fat, meditteranean etc.; can include multiple diet types}	{Name of relevant disease, e.g. Salmonella gastroenteritis. Controlled vocabulary, http://bioportal.bioontology.org/ontologies/1009 or http://www.ncbi.nlm.nih.gov/mesh}	{none}	{none}	{the height of subject}	{HIV status of subject, if yes HAART initiation status should also be indicated as [YES or NO]}	{content of last meal and time since feeding; can include multiple values}	{most frequent job performed by subject}	{none}	{resting pulse of the host, measured as beats per minute}	{Gender or physical sex of the host}	{a unique identifier by which each subject can be referred to, de-identified, e.g. #131}	{Type of tissue the initial sample was taken from. Controlled vocabulary, http://bioportal.bioontology.org/ontologies/1005)}	{total mass of the host at collection, the unit depends on host}	{can include multiple medication codes}	{history of kidney disorders; can include multiple disorders}	{specification of the maternal health status}	{whether full medical history was collected}	{any other measurement performed or parameter collected, that is not listed here}	{history of nose-throat disorders; can include multiple disorders}	{total count of any organism per gram or volume of sample,should include name of organism followed by count; can include multiple organism counts}	{oxygenation status of sample}	{type of perturbation, e.g. chemical administration, physical disturbance, etc., coupled with time that perturbation occurred; can include multiple perturbation types}	{specification of presence of pets or farm animals in the environment of subject, if yes the animals should be specified; can include multiple animals present}	{history of pulmonary disorders; can include multiple disorders}	{none}	{none}	{none}	{none}	{specification of smoking status}	{specification of study completion status, if no the reason should be specified}	{temperature of the sample at time of sampling}	{specification of the countries travelled in the last six months; can include multiple travels}	{specification of twin sibling presence}	{specification of urine collection method}	{history of urogenitaltract disorders; can include multiple disorders}	{specification of weight loss in the last three months, if yes should be further specified to include amount of weight loss}" << endl;
                out << "#{text}	{text}	{text}	{{timestamp}}	{{term}}	{{term}}	{{term}}	{{term}:{term}:{text}}	{{float} {float}}	{{text}}	{{text}}	{{term}; {timestamp}}	{{boolean};{text}}	{{text};{integer}/[year|month|week|day|hour]}	{{integer|text}}	{{text}}	{{text}}	{{none}}	{{float} {unit}}	{{text}}	{{float} {unit}}	{{text}}	{{none}}	{{none}}	{{none}}	{{float} {unit}}	{{boolean};{boolean}}	{{text};{period}}	{{none}}	{{none}}	{{float} {unit}}	{['', 'male', 'female', 'pooled male and female', 'neuter', 'hermaphrodite', 'not determined', 'missing', 'not applicable', 'not collected']}	{{text}}	{{none}}	{{float} {unit}}	{{integer}}	{{text}}	{{text}}	{{boolean}}	{{text};{float} {unit}}	{{text}}	{{text};{float} {unit}}	{['', 'aerobic', 'anaerobic']}	{{text};{interval}}	{{boolean};{text}}	{{text}}	{{none}}	{{none}}	{{none}}	{{none}}	{{boolean}}	{{boolean};[adverse event|non-compliance|lost to follow up|other-specify]}	{{float} {unit}}	{{text}}	{{boolean}}	{['', 'clean catch', 'catheter']}	{{text}}	{{boolean};{float} {unit}}" << endl;
                out << "*sample_name	*description	*sample_title	*collection_date	*env_biome	*env_feature	*env_material	*geo_loc_name	*lat_lon	amniotic_fluid_color	blood_blood_disord	chem_administration	diet_last_six_month	drug_usage	ethnicity	foetal_health_stat	gestation_state	host_age	host_body_mass_index	host_body_product	host_body_temp	host_diet	host_disease	host_family_relationship	host_genotype	host_height	host_hiv_stat	host_last_meal	host_occupation	host_phenotype	host_pulse	host_sex	host_subject_id	host_tissue_sampled	host_tot_mass	ihmc_medication_code	kidney_disord	maternal_health_stat	medic_hist_perform	misc_param	nose_throat_disord	organism_count	oxy_stat_samp	perturbation	pet_farm_animal	pulmonary_disord	samp_salinity	samp_store_dur	samp_store_loc	samp_store_temp	smoker	study_complt_stat	temp	travel_out_six_month	twin_sibling	urine_collect_meth	urogenit_tract_disor	weight_loss_3_month" << endl;
            }
        }else if (package == "Human_gut") {
            out << "#MIMARKS.survey.human-gut.4.0" << endl;
            if (requiredonly) {
                out << "#{sample name}	{description of sample}	{sample title}	{Date of sampling, in \"DD-Mmm-YYYY\", \"Mmm-YYYY\" or \"YYYY\" format (eg., 30-Oct-1990, Oct-1990 or 1990) or ISO 8601 standard \"YYYY-mm-dd\", \"YYYY-mm\" or \"YYYY-mm-ddThh:mm:ss\" (eg., 1990-10-30, 1990-10 or 1990-10-30T14:41:36)}	{descriptor of the broad ecological context of a sample. Examples include: desert, taiga, deciduous woodland, or coral reef. EnvO (v 2013-06-14) terms can be found via the link: www.environmentontology.org/Browse-EnvO}	{descriptor of the local environment. Examples include: harbor, cliff, or lake. EnvO (v 2013-06-14) terms can be found via the link: www.environmentontology.org/Browse-EnvO}	{material that was displaced by the sample, or material in which a sample was embedded, prior to the sampling event. Examples include: air, soil, or water. EnvO (v 2013-06-14) terms can be found via the link: www.environmentontology.org/Browse-EnvO}	{Geographical origin of the sample; use the appropriate name from this list http://www.insdc.org/documents/country-qualifier-vocabulary. Use a colon to separate the country or ocean from more detailed information about the location, eg \"Canada: Vancouver\" or \"Germany: halfway down Zugspitze, Alps\"}	{The geographical coordinates of the location where the sample was collected. Specify as degrees latitude and longitude in format \"d[d.dddd] N|S d[dd.dddd] W|E\", eg, 38.98 N 77.11 W}" << endl;
                out << "#{text}	{text}	{text}	{{timestamp}}	{{term}}	{{term}}	{{term}}	{{term}:{term}:{text}}	{{float} {float}}" << endl;
                out << "*sample_name	*description	*sample_title	*collection_date	*env_biome	*env_feature	*env_material	*geo_loc_name	*lat_lon" << endl;
            }else {
                out << "#{sample name}	{description of sample}	{sample title}	{Date of sampling, in \"DD-Mmm-YYYY\", \"Mmm-YYYY\" or \"YYYY\" format (eg., 30-Oct-1990, Oct-1990 or 1990) or ISO 8601 standard \"YYYY-mm-dd\", \"YYYY-mm\" or \"YYYY-mm-ddThh:mm:ss\" (eg., 1990-10-30, 1990-10 or 1990-10-30T14:41:36)}	{descriptor of the broad ecological context of a sample. Examples include: desert, taiga, deciduous woodland, or coral reef. EnvO (v 2013-06-14) terms can be found via the link: www.environmentontology.org/Browse-EnvO}	{descriptor of the local environment. Examples include: harbor, cliff, or lake. EnvO (v 2013-06-14) terms can be found via the link: www.environmentontology.org/Browse-EnvO}	{material that was displaced by the sample, or material in which a sample was embedded, prior to the sampling event. Examples include: air, soil, or water. EnvO (v 2013-06-14) terms can be found via the link: www.environmentontology.org/Browse-EnvO}	{Geographical origin of the sample; use the appropriate name from this list http://www.insdc.org/documents/country-qualifier-vocabulary. Use a colon to separate the country or ocean from more detailed information about the location, eg \"Canada: Vancouver\" or \"Germany: halfway down Zugspitze, Alps\"}	{The geographical coordinates of the location where the sample was collected. Specify as degrees latitude and longitude in format \"d[d.dddd] N|S d[dd.dddd] W|E\", eg, 38.98 N 77.11 W}	{list of chemical compounds administered to the host or site where sampling occurred, and when (e.g. antibiotics, N fertilizer, air filter); can include multiple compounds. For Chemical Entities of Biological Interest ontology (CHEBI) (v1.72), please see http://bioportal.bioontology.org/visualize/44603}	{ethnicity of the subject}	{history of gastrointestinal tract disorders; can include multiple disorders}	{Age of host at the time of sampling}	{body mass index of the host, calculated as weight/(height)squared}	{substance produced by the host, e.g. stool, mucus, where the sample was obtained from}	{core body temperature of the host when sample was collected}	{type of diet depending on the sample for animals omnivore, herbivore etc., for humans high-fat, meditteranean etc.; can include multiple diet types}	{Name of relevant disease, e.g. Salmonella gastroenteritis. Controlled vocabulary, http://bioportal.bioontology.org/ontologies/1009 or http://www.ncbi.nlm.nih.gov/mesh}	{none}	{none}	{the height of subject}	{content of last meal and time since feeding; can include multiple values}	{most frequent job performed by subject}	{none}	{resting pulse of the host, measured as beats per minute}	{Gender or physical sex of the host}	{a unique identifier by which each subject can be referred to, de-identified, e.g. #131}	{Type of tissue the initial sample was taken from. Controlled vocabulary, http://bioportal.bioontology.org/ontologies/1005)}	{total mass of the host at collection, the unit depends on host}	{can include multiple medication codes}	{history of liver disorders; can include multiple disorders}	{whether full medical history was collected}	{any other measurement performed or parameter collected, that is not listed here}	{total count of any organism per gram or volume of sample,should include name of organism followed by count; can include multiple organism counts}	{oxygenation status of sample}	{type of perturbation, e.g. chemical administration, physical disturbance, etc., coupled with time that perturbation occurred; can include multiple perturbation types}	{none}	{none}	{none}	{none}	{specification of special diet; can include multiple special diets}	{temperature of the sample at time of sampling}" << endl;
                out << "#{text}	{text}	{text}	{{timestamp}}	{{term}}	{{term}}	{{term}}	{{term}:{term}:{text}}	{{float} {float}}	{{term}; {timestamp}}	{{integer|text}}	{{text}}	{{none}}	{{float} {unit}}	{{text}}	{{float} {unit}}	{{text}}	{{none}}	{{none}}	{{none}}	{{float} {unit}}	{{text};{period}}	{{none}}	{{none}}	{{float} {unit}}	{['', 'male', 'female', 'pooled male and female', 'neuter', 'hermaphrodite', 'not determined', 'missing', 'not applicable', 'not collected']}	{{text}}	{{none}}	{{float} {unit}}	{{integer}}	{{text}}	{{boolean}}	{{text};{float} {unit}}	{{text};{float} {unit}}	{['', 'aerobic', 'anaerobic']}	{{text};{interval}}	{{none}}	{{none}}	{{none}}	{{none}}	{['', 'low carb', 'reduced calorie', 'vegetarian', 'other(to be specified)']}	{{float} {unit}}" << endl;
                out << "*sample_name	*description	*sample_title	*collection_date	*env_biome	*env_feature	*env_material	*geo_loc_name	*lat_lon	chem_administration	ethnicity	gastrointest_disord	host_age	host_body_mass_index	host_body_product	host_body_temp	host_diet	host_disease	host_family_relationship	host_genotype	host_height	host_last_meal	host_occupation	host_phenotype	host_pulse	host_sex	host_subject_id	host_tissue_sampled	host_tot_mass	ihmc_medication_code	liver_disord	medic_hist_perform	misc_param	organism_count	oxy_stat_samp	perturbation	samp_salinity	samp_store_dur	samp_store_loc	samp_store_temp	special_diet	temp" << endl;
            }
        }else if (package == "Human_oral") {
            out << "#MIMARKS.survey.human-oral.4.0" << endl;
            if (requiredonly) {
                out << "#{sample name}	{description of sample}	{sample title}	{Date of sampling, in \"DD-Mmm-YYYY\", \"Mmm-YYYY\" or \"YYYY\" format (eg., 30-Oct-1990, Oct-1990 or 1990) or ISO 8601 standard \"YYYY-mm-dd\", \"YYYY-mm\" or \"YYYY-mm-ddThh:mm:ss\" (eg., 1990-10-30, 1990-10 or 1990-10-30T14:41:36)}	{descriptor of the broad ecological context of a sample. Examples include: desert, taiga, deciduous woodland, or coral reef. EnvO (v 2013-06-14) terms can be found via the link: www.environmentontology.org/Browse-EnvO}	{descriptor of the local environment. Examples include: harbor, cliff, or lake. EnvO (v 2013-06-14) terms can be found via the link: www.environmentontology.org/Browse-EnvO}	{material that was displaced by the sample, or material in which a sample was embedded, prior to the sampling event. Examples include: air, soil, or water. EnvO (v 2013-06-14) terms can be found via the link: www.environmentontology.org/Browse-EnvO}	{Geographical origin of the sample; use the appropriate name from this list http://www.insdc.org/documents/country-qualifier-vocabulary. Use a colon to separate the country or ocean from more detailed information about the location, eg \"Canada: Vancouver\" or \"Germany: halfway down Zugspitze, Alps\"}	{The geographical coordinates of the location where the sample was collected. Specify as degrees latitude and longitude in format \"d[d.dddd] N|S d[dd.dddd] W|E\", eg, 38.98 N 77.11 W}" << endl;
                out << "#{text}	{text}	{text}	{{timestamp}}	{{term}}	{{term}}	{{term}}	{{term}:{term}:{text}}	{{float} {float}}" << endl;
                out << "*sample_name	*description	*sample_title	*collection_date	*env_biome	*env_feature	*env_material	*geo_loc_name	*lat_lon" << endl;
            }else {
                out << "#{sample name}	{description of sample}	{sample title}	{Date of sampling, in \"DD-Mmm-YYYY\", \"Mmm-YYYY\" or \"YYYY\" format (eg., 30-Oct-1990, Oct-1990 or 1990) or ISO 8601 standard \"YYYY-mm-dd\", \"YYYY-mm\" or \"YYYY-mm-ddThh:mm:ss\" (eg., 1990-10-30, 1990-10 or 1990-10-30T14:41:36)}	{descriptor of the broad ecological context of a sample. Examples include: desert, taiga, deciduous woodland, or coral reef. EnvO (v 2013-06-14) terms can be found via the link: www.environmentontology.org/Browse-EnvO}	{descriptor of the local environment. Examples include: harbor, cliff, or lake. EnvO (v 2013-06-14) terms can be found via the link: www.environmentontology.org/Browse-EnvO}	{material that was displaced by the sample, or material in which a sample was embedded, prior to the sampling event. Examples include: air, soil, or water. EnvO (v 2013-06-14) terms can be found via the link: www.environmentontology.org/Browse-EnvO}	{Geographical origin of the sample; use the appropriate name from this list http://www.insdc.org/documents/country-qualifier-vocabulary. Use a colon to separate the country or ocean from more detailed information about the location, eg \"Canada: Vancouver\" or \"Germany: halfway down Zugspitze, Alps\"}	{The geographical coordinates of the location where the sample was collected. Specify as degrees latitude and longitude in format \"d[d.dddd] N|S d[dd.dddd] W|E\", eg, 38.98 N 77.11 W}	{list of chemical compounds administered to the host or site where sampling occurred, and when (e.g. antibiotics, N fertilizer, air filter); can include multiple compounds. For Chemical Entities of Biological Interest ontology (CHEBI) (v1.72), please see http://bioportal.bioontology.org/visualize/44603}	{ethnicity of the subject}	{Age of host at the time of sampling}	{body mass index of the host, calculated as weight/(height)squared}	{substance produced by the host, e.g. stool, mucus, where the sample was obtained from}	{core body temperature of the host when sample was collected}	{type of diet depending on the sample for animals omnivore, herbivore etc., for humans high-fat, meditteranean etc.; can include multiple diet types}	{Name of relevant disease, e.g. Salmonella gastroenteritis. Controlled vocabulary, http://bioportal.bioontology.org/ontologies/1009 or http://www.ncbi.nlm.nih.gov/mesh}	{none}	{none}	{the height of subject}	{content of last meal and time since feeding; can include multiple values}	{most frequent job performed by subject}	{none}	{resting pulse of the host, measured as beats per minute}	{Gender or physical sex of the host}	{a unique identifier by which each subject can be referred to, de-identified, e.g. #131}	{Type of tissue the initial sample was taken from. Controlled vocabulary, http://bioportal.bioontology.org/ontologies/1005)}	{total mass of the host at collection, the unit depends on host}	{can include multiple medication codes}	{whether full medical history was collected}	{any other measurement performed or parameter collected, that is not listed here}	{history of nose/mouth/teeth/throat disorders; can include multiple disorders}	{total count of any organism per gram or volume of sample,should include name of organism followed by count; can include multiple organism counts}	{oxygenation status of sample}	{type of perturbation, e.g. chemical administration, physical disturbance, etc., coupled with time that perturbation occurred; can include multiple perturbation types}	{none}	{none}	{none}	{none}	{temperature of the sample at time of sampling}	{specification of the time since last toothbrushing}" << endl;
                out << "#{text}	{text}	{text}	{{timestamp}}	{{term}}	{{term}}	{{term}}	{{term}:{term}:{text}}	{{float} {float}}	{{term}; {timestamp}}	{{integer|text}}	{{none}}	{{float} {unit}}	{{text}}	{{float} {unit}}	{{text}}	{{none}}	{{none}}	{{none}}	{{float} {unit}}	{{text};{period}}	{{none}}	{{none}}	{{float} {unit}}	{['', 'male', 'female', 'pooled male and female', 'neuter', 'hermaphrodite', 'not determined', 'missing', 'not applicable', 'not collected']}	{{text}}	{{none}}	{{float} {unit}}	{{integer}}	{{boolean}}	{{text};{float} {unit}}	{{text}}	{{text};{float} {unit}}	{['', 'aerobic', 'anaerobic']}	{{text};{interval}}	{{none}}	{{none}}	{{none}}	{{none}}	{{float} {unit}}	{{timestamp}}" << endl;
                out << "*sample_name	*description	*sample_title	*collection_date	*env_biome	*env_feature	*env_material	*geo_loc_name	*lat_lon	chem_administration	ethnicity	host_age	host_body_mass_index	host_body_product	host_body_temp	host_diet	host_disease	host_family_relationship	host_genotype	host_height	host_last_meal	host_occupation	host_phenotype	host_pulse	host_sex	host_subject_id	host_tissue_sampled	host_tot_mass	ihmc_medication_code	medic_hist_perform	misc_param	nose_mouth_teeth_throat_disord	organism_count	oxy_stat_samp	perturbation	samp_salinity	samp_store_dur	samp_store_loc	samp_store_temp	temp	time_last_toothbrush" << endl;
            }
        }else if (package == "Human_skin") {
            out << "#MIMARKS.survey.human-skin.4.0" << endl;
            if (requiredonly) {
                out << "#{sample name}	{description of sample}	{sample title}	{Date of sampling, in \"DD-Mmm-YYYY\", \"Mmm-YYYY\" or \"YYYY\" format (eg., 30-Oct-1990, Oct-1990 or 1990) or ISO 8601 standard \"YYYY-mm-dd\", \"YYYY-mm\" or \"YYYY-mm-ddThh:mm:ss\" (eg., 1990-10-30, 1990-10 or 1990-10-30T14:41:36)}	{descriptor of the broad ecological context of a sample. Examples include: desert, taiga, deciduous woodland, or coral reef. EnvO (v 2013-06-14) terms can be found via the link: www.environmentontology.org/Browse-EnvO}	{descriptor of the local environment. Examples include: harbor, cliff, or lake. EnvO (v 2013-06-14) terms can be found via the link: www.environmentontology.org/Browse-EnvO}	{material that was displaced by the sample, or material in which a sample was embedded, prior to the sampling event. Examples include: air, soil, or water. EnvO (v 2013-06-14) terms can be found via the link: www.environmentontology.org/Browse-EnvO}	{Geographical origin of the sample; use the appropriate name from this list http://www.insdc.org/documents/country-qualifier-vocabulary. Use a colon to separate the country or ocean from more detailed information about the location, eg \"Canada: Vancouver\" or \"Germany: halfway down Zugspitze, Alps\"}	{The geographical coordinates of the location where the sample was collected. Specify as degrees latitude and longitude in format \"d[d.dddd] N|S d[dd.dddd] W|E\", eg, 38.98 N 77.11 W}" << endl;
                out << "#{text}	{text}	{text}	{{timestamp}}	{{term}}	{{term}}	{{term}}	{{term}:{term}:{text}}	{{float} {float}}" << endl;
                out << "*sample_name	*description	*sample_title	*collection_date	*env_biome	*env_feature	*env_material	*geo_loc_name	*lat_lon" << endl;
            }else {
                out << "#{sample name}	{description of sample}	{sample title}	{Date of sampling, in \"DD-Mmm-YYYY\", \"Mmm-YYYY\" or \"YYYY\" format (eg., 30-Oct-1990, Oct-1990 or 1990) or ISO 8601 standard \"YYYY-mm-dd\", \"YYYY-mm\" or \"YYYY-mm-ddThh:mm:ss\" (eg., 1990-10-30, 1990-10 or 1990-10-30T14:41:36)}	{descriptor of the broad ecological context of a sample. Examples include: desert, taiga, deciduous woodland, or coral reef. EnvO (v 2013-06-14) terms can be found via the link: www.environmentontology.org/Browse-EnvO}	{descriptor of the local environment. Examples include: harbor, cliff, or lake. EnvO (v 2013-06-14) terms can be found via the link: www.environmentontology.org/Browse-EnvO}	{material that was displaced by the sample, or material in which a sample was embedded, prior to the sampling event. Examples include: air, soil, or water. EnvO (v 2013-06-14) terms can be found via the link: www.environmentontology.org/Browse-EnvO}	{Geographical origin of the sample; use the appropriate name from this list http://www.insdc.org/documents/country-qualifier-vocabulary. Use a colon to separate the country or ocean from more detailed information about the location, eg \"Canada: Vancouver\" or \"Germany: halfway down Zugspitze, Alps\"}	{The geographical coordinates of the location where the sample was collected. Specify as degrees latitude and longitude in format \"d[d.dddd] N|S d[dd.dddd] W|E\", eg, 38.98 N 77.11 W}	{list of chemical compounds administered to the host or site where sampling occurred, and when (e.g. antibiotics, N fertilizer, air filter); can include multiple compounds. For Chemical Entities of Biological Interest ontology (CHEBI) (v1.72), please see http://bioportal.bioontology.org/visualize/44603}	{history of dermatology disorders; can include multiple disorders}	{dominant hand of the subject}	{ethnicity of the subject}	{Age of host at the time of sampling}	{body mass index of the host, calculated as weight/(height)squared}	{substance produced by the host, e.g. stool, mucus, where the sample was obtained from}	{core body temperature of the host when sample was collected}	{type of diet depending on the sample for animals omnivore, herbivore etc., for humans high-fat, meditteranean etc.; can include multiple diet types}	{Name of relevant disease, e.g. Salmonella gastroenteritis. Controlled vocabulary, http://bioportal.bioontology.org/ontologies/1009 or http://www.ncbi.nlm.nih.gov/mesh}	{none}	{none}	{the height of subject}	{content of last meal and time since feeding; can include multiple values}	{most frequent job performed by subject}	{none}	{resting pulse of the host, measured as beats per minute}	{Gender or physical sex of the host}	{a unique identifier by which each subject can be referred to, de-identified, e.g. #131}	{Type of tissue the initial sample was taken from. Controlled vocabulary, http://bioportal.bioontology.org/ontologies/1005)}	{total mass of the host at collection, the unit depends on host}	{can include multiple medication codes}	{whether full medical history was collected}	{any other measurement performed or parameter collected, that is not listed here}	{total count of any organism per gram or volume of sample,should include name of organism followed by count; can include multiple organism counts}	{oxygenation status of sample}	{type of perturbation, e.g. chemical administration, physical disturbance, etc., coupled with time that perturbation occurred; can include multiple perturbation types}	{none}	{none}	{none}	{none}	{temperature of the sample at time of sampling}	{specification of the time since last wash}" << endl;
                out << "#{text}	{text}	{text}	{{timestamp}}	{{term}}	{{term}}	{{term}}	{{term}:{term}:{text}}	{{float} {float}}	{{term}; {timestamp}}	{{text}}	{['', 'left', 'right', 'ambidextrous']}	{{integer|text}}	{{none}}	{{float} {unit}}	{{text}}	{{float} {unit}}	{{text}}	{{none}}	{{none}}	{{none}}	{{float} {unit}}	{{text};{period}}	{{none}}	{{none}}	{{float} {unit}}	{['', 'male', 'female', 'pooled male and female', 'neuter', 'hermaphrodite', 'not determined', 'missing', 'not applicable', 'not collected']}	{{text}}	{{none}}	{{float} {unit}}	{{integer}}	{{boolean}}	{{text};{float} {unit}}	{{text};{float} {unit}}	{['', 'aerobic', 'anaerobic']}	{{text};{interval}}	{{none}}	{{none}}	{{none}}	{{none}}	{{float} {unit}}	{{timestamp}}" << endl;
                out << "*sample_name	*description	*sample_title	*collection_date	*env_biome	*env_feature	*env_material	*geo_loc_name	*lat_lon	chem_administration	dermatology_disord	dominant_hand	ethnicity	host_age	host_body_mass_index	host_body_product	host_body_temp	host_diet	host_disease	host_family_relationship	host_genotype	host_height	host_last_meal	host_occupation	host_phenotype	host_pulse	host_sex	host_subject_id	host_tissue_sampled	host_tot_mass	ihmc_medication_code	medic_hist_perform	misc_param	organism_count	oxy_stat_samp	perturbation	samp_salinity	samp_store_dur	samp_store_loc	samp_store_temp	temp	time_since_last_wash" << endl;
            }
        }else if (package == "Human_vaginal") {
            out << "#MIMARKS.survey.human-vaginal.4.0" << endl;
            if (requiredonly) {
                out << "#{sample name}	{description of sample}	{sample title}	{Date of sampling, in \"DD-Mmm-YYYY\", \"Mmm-YYYY\" or \"YYYY\" format (eg., 30-Oct-1990, Oct-1990 or 1990) or ISO 8601 standard \"YYYY-mm-dd\", \"YYYY-mm\" or \"YYYY-mm-ddThh:mm:ss\" (eg., 1990-10-30, 1990-10 or 1990-10-30T14:41:36)}	{descriptor of the broad ecological context of a sample. Examples include: desert, taiga, deciduous woodland, or coral reef. EnvO (v 2013-06-14) terms can be found via the link: www.environmentontology.org/Browse-EnvO}	{descriptor of the local environment. Examples include: harbor, cliff, or lake. EnvO (v 2013-06-14) terms can be found via the link: www.environmentontology.org/Browse-EnvO}	{material that was displaced by the sample, or material in which a sample was embedded, prior to the sampling event. Examples include: air, soil, or water. EnvO (v 2013-06-14) terms can be found via the link: www.environmentontology.org/Browse-EnvO}	{Geographical origin of the sample; use the appropriate name from this list http://www.insdc.org/documents/country-qualifier-vocabulary. Use a colon to separate the country or ocean from more detailed information about the location, eg \"Canada: Vancouver\" or \"Germany: halfway down Zugspitze, Alps\"}	{The geographical coordinates of the location where the sample was collected. Specify as degrees latitude and longitude in format \"d[d.dddd] N|S d[dd.dddd] W|E\", eg, 38.98 N 77.11 W}" << endl;
                out << "#{text}	{text}	{text}	{{timestamp}}	{{term}}	{{term}}	{{term}}	{{term}:{term}:{text}}	{{float} {float}}" << endl;
                out << "*sample_name	*description	*sample_title	*collection_date	*env_biome	*env_feature	*env_material	*geo_loc_name	*lat_lon" << endl;
            }else {
                out << "#{sample name}	{description of sample}	{sample title}	{Date of sampling, in \"DD-Mmm-YYYY\", \"Mmm-YYYY\" or \"YYYY\" format (eg., 30-Oct-1990, Oct-1990 or 1990) or ISO 8601 standard \"YYYY-mm-dd\", \"YYYY-mm\" or \"YYYY-mm-ddThh:mm:ss\" (eg., 1990-10-30, 1990-10 or 1990-10-30T14:41:36)}	{descriptor of the broad ecological context of a sample. Examples include: desert, taiga, deciduous woodland, or coral reef. EnvO (v 2013-06-14) terms can be found via the link: www.environmentontology.org/Browse-EnvO}	{descriptor of the local environment. Examples include: harbor, cliff, or lake. EnvO (v 2013-06-14) terms can be found via the link: www.environmentontology.org/Browse-EnvO}	{material that was displaced by the sample, or material in which a sample was embedded, prior to the sampling event. Examples include: air, soil, or water. EnvO (v 2013-06-14) terms can be found via the link: www.environmentontology.org/Browse-EnvO}	{Geographical origin of the sample; use the appropriate name from this list http://www.insdc.org/documents/country-qualifier-vocabulary. Use a colon to separate the country or ocean from more detailed information about the location, eg \"Canada: Vancouver\" or \"Germany: halfway down Zugspitze, Alps\"}	{The geographical coordinates of the location where the sample was collected. Specify as degrees latitude and longitude in format \"d[d.dddd] N|S d[dd.dddd] W|E\", eg, 38.98 N 77.11 W}	{specification of birth control medication used}	{list of chemical compounds administered to the host or site where sampling occurred, and when (e.g. antibiotics, N fertilizer, air filter); can include multiple compounds. For Chemical Entities of Biological Interest ontology (CHEBI) (v1.72), please see http://bioportal.bioontology.org/visualize/44603}	{date of most recent douche}	{ethnicity of the subject}	{history of gynecological disorders; can include multiple disorders}	{Age of host at the time of sampling}	{body mass index of the host, calculated as weight/(height)squared}	{substance produced by the host, e.g. stool, mucus, where the sample was obtained from}	{core body temperature of the host when sample was collected}	{type of diet depending on the sample for animals omnivore, herbivore etc., for humans high-fat, meditteranean etc.; can include multiple diet types}	{Name of relevant disease, e.g. Salmonella gastroenteritis. Controlled vocabulary, http://bioportal.bioontology.org/ontologies/1009 or http://www.ncbi.nlm.nih.gov/mesh}	{none}	{none}	{the height of subject}	{content of last meal and time since feeding; can include multiple values}	{most frequent job performed by subject}	{none}	{resting pulse of the host, measured as beats per minute}	{Gender or physical sex of the host}	{a unique identifier by which each subject can be referred to, de-identified, e.g. #131}	{Type of tissue the initial sample was taken from. Controlled vocabulary, http://bioportal.bioontology.org/ontologies/1005)}	{total mass of the host at collection, the unit depends on host}	{whether subject had hormone replacement theraphy, and if yes start date}	{specification of whether hysterectomy was performed}	{can include multiple medication codes}	{whether full medical history was collected}	{date of most recent menstruation}	{date of onset of menopause}	{any other measurement performed or parameter collected, that is not listed here}	{total count of any organism per gram or volume of sample,should include name of organism followed by count; can include multiple organism counts}	{oxygenation status of sample}	{type of perturbation, e.g. chemical administration, physical disturbance, etc., coupled with time that perturbation occurred; can include multiple perturbation types}	{date due of pregnancy}	{none}	{none}	{none}	{none}	{current sexual partner and frequency of sex}	{temperature of the sample at time of sampling}	{history of urogenital disorders, can include multiple disorders}" << endl;
                out << "#{text}	{text}	{text}	{{timestamp}}	{{term}}	{{term}}	{{term}}	{{term}:{term}:{text}}	{{float} {float}}	{{text}}	{{term}; {timestamp}}	{{timestamp}}	{{integer|text}}	{{text}}	{{none}}	{{float} {unit}}	{{text}}	{{float} {unit}}	{{text}}	{{none}}	{{none}}	{{none}}	{{float} {unit}}	{{text};{period}}	{{none}}	{{none}}	{{float} {unit}}	{['', 'male', 'female', 'pooled male and female', 'neuter', 'hermaphrodite', 'not determined', 'missing', 'not applicable', 'not collected']}	{{text}}	{{none}}	{{float} {unit}}	{{timestamp}}	{{boolean}}	{{integer}}	{{boolean}}	{{timestamp}}	{{timestamp}}	{{text};{float} {unit}}	{{text};{float} {unit}}	{['', 'aerobic', 'anaerobic']}	{{text};{interval}}	{{timestamp}}	{{none}}	{{none}}	{{none}}	{{none}}	{{text}}	{{float} {unit}}	{{text}}" << endl;
                out << "*sample_name	*description	*sample_title	*collection_date	*env_biome	*env_feature	*env_material	*geo_loc_name	*lat_lon	birth_control	chem_administration	douche	ethnicity	gynecologic_disord	host_age	host_body_mass_index	host_body_product	host_body_temp	host_diet	host_disease	host_family_relationship	host_genotype	host_height	host_last_meal	host_occupation	host_phenotype	host_pulse	host_sex	host_subject_id	host_tissue_sampled	host_tot_mass	hrt	hysterectomy	ihmc_medication_code	medic_hist_perform	menarche	menopause	misc_param	organism_count	oxy_stat_samp	perturbation	pregnancy	samp_salinity	samp_store_dur	samp_store_loc	samp_store_temp	sexual_act	temp	urogenit_disord" << endl;
            }
        }else if (package == "Microbial") {
            out << "#MIMARKS.survey.microbial.4.0" << endl;
            if (requiredonly) {
                out << "#{sample name}	{description of sample}	{sample title}	{Date of sampling, in \"DD-Mmm-YYYY\", \"Mmm-YYYY\" or \"YYYY\" format (eg., 30-Oct-1990, Oct-1990 or 1990) or ISO 8601 standard \"YYYY-mm-dd\", \"YYYY-mm\" or \"YYYY-mm-ddThh:mm:ss\" (eg., 1990-10-30, 1990-10 or 1990-10-30T14:41:36)}	{descriptor of the broad ecological context of a sample. Examples include: desert, taiga, deciduous woodland, or coral reef. EnvO (v 2013-06-14) terms can be found via the link: www.environmentontology.org/Browse-EnvO}	{descriptor of the local environment. Examples include: harbor, cliff, or lake. EnvO (v 2013-06-14) terms can be found via the link: www.environmentontology.org/Browse-EnvO}	{material that was displaced by the sample, or material in which a sample was embedded, prior to the sampling event. Examples include: air, soil, or water. EnvO (v 2013-06-14) terms can be found via the link: www.environmentontology.org/Browse-EnvO}	{Geographical origin of the sample; use the appropriate name from this list http://www.insdc.org/documents/country-qualifier-vocabulary. Use a colon to separate the country or ocean from more detailed information about the location, eg \"Canada: Vancouver\" or \"Germany: halfway down Zugspitze, Alps\"}	{The geographical coordinates of the location where the sample was collected. Specify as degrees latitude and longitude in format \"d[d.dddd] N|S d[dd.dddd] W|E\", eg, 38.98 N 77.11 W}	{Depth is defined as the vertical distance below surface, e.g. for sediment or soil samples depth is measured from sediment or soil surface, respectivly. Depth can be reported as an interval for subsurface samples.}	{The elevation of the sampling site as measured by the vertical distance from mean sea level.}" << endl;
                out << "#{text}	{text}	{text}	{{timestamp}}	{{term}}	{{term}}	{{term}}	{{term}:{term}:{text}}	{{float} {float}}	{{float} m}	{{float} {unit}}" << endl;
                out << "*sample_name	*description	*sample_title	*collection_date	*env_biome	*env_feature	*env_material	*geo_loc_name	*lat_lon	*depth	*elev" << endl;
            }else {
                out << "#{sample name}	{description of sample}	{sample title}	{Date of sampling, in \"DD-Mmm-YYYY\", \"Mmm-YYYY\" or \"YYYY\" format (eg., 30-Oct-1990, Oct-1990 or 1990) or ISO 8601 standard \"YYYY-mm-dd\", \"YYYY-mm\" or \"YYYY-mm-ddThh:mm:ss\" (eg., 1990-10-30, 1990-10 or 1990-10-30T14:41:36)}	{descriptor of the broad ecological context of a sample. Examples include: desert, taiga, deciduous woodland, or coral reef. EnvO (v 2013-06-14) terms can be found via the link: www.environmentontology.org/Browse-EnvO}	{descriptor of the local environment. Examples include: harbor, cliff, or lake. EnvO (v 2013-06-14) terms can be found via the link: www.environmentontology.org/Browse-EnvO}	{material that was displaced by the sample, or material in which a sample was embedded, prior to the sampling event. Examples include: air, soil, or water. EnvO (v 2013-06-14) terms can be found via the link: www.environmentontology.org/Browse-EnvO}	{Geographical origin of the sample; use the appropriate name from this list http://www.insdc.org/documents/country-qualifier-vocabulary. Use a colon to separate the country or ocean from more detailed information about the location, eg \"Canada: Vancouver\" or \"Germany: halfway down Zugspitze, Alps\"}	{The geographical coordinates of the location where the sample was collected. Specify as degrees latitude and longitude in format \"d[d.dddd] N|S d[dd.dddd] W|E\", eg, 38.98 N 77.11 W}	{Depth is defined as the vertical distance below surface, e.g. for sediment or soil samples depth is measured from sediment or soil surface, respectivly. Depth can be reported as an interval for subsurface samples.}	{The elevation of the sampling site as measured by the vertical distance from mean sea level.}	{alkalinity, the ability of a solution to neutralize acids to the equivalence point of carbonate or bicarbonate}	{concentration of alkyl diethers}	{The altitude of the sample is the vertical distance between Earth's surface above Sea Level and the sampled position in the air.}	{measurement of aminopeptidase activity}	{concentration of ammonium}	{measurement of bacterial carbon production}	{amount of biomass; should include the name for the part of biomass measured, e.g. microbial, total. can include multiple measurements}	{concentration of bishomohopanol}	{concentration of bromide}	{concentration of calcium}	{ratio of amount or concentrations of carbon to nitrogen}	{list of chemical compounds administered to the host or site where sampling occurred, and when (e.g. antibiotics, N fertilizer, air filter); can include multiple compounds. For Chemical Entities of Biological Interest ontology (CHEBI) (v1.72), please see http://bioportal.bioontology.org/visualize/44603}	{concentration of chloride}	{concentration of chlorophyll}	{concentration of diether lipids; can include multiple types of diether lipids}	{concentration of dissolved carbon dioxide}	{concentration of dissolved hydrogen}	{dissolved inorganic carbon concentration}	{concentration of dissolved organic carbon}	{dissolved organic nitrogen concentration measured as; total dissolved nitrogen - NH4 - NO3 - NO2}	{concentration of dissolved oxygen}	{measurement of glucosidase activity}	{concentration of magnesium}	{measurement of mean friction velocity}	{measurement of mean peak friction velocity}	{methane (gas) amount or concentration at the time of sampling}	{any other measurement performed or parameter collected, that is not listed here}	{concentration of n-alkanes; can include multiple n-alkanes}	{concentration of nitrate}	{concentration of nitrite}	{concentration of nitrogen (total)}	{concentration of organic carbon}	{concentration of organic matter}	{concentration of organic nitrogen}	{total count of any organism per gram or volume of sample,should include name of organism followed by count; can include multiple organism counts}	{oxygenation status of sample}	{concentration of particulate organic carbon}	{type of perturbation, e.g. chemical administration, physical disturbance, etc., coupled with time that perturbation occurred; can include multiple perturbation types}	{concentration of petroleum hydrocarbon}	{pH measurement}	{concentration of phaeopigments; can include multiple phaeopigments}	{concentration of phosphate}	{concentration of phospholipid fatty acids; can include multiple values}	{concentration of potassium}	{pressure to which the sample is subject, in atmospheres}	{redox potential, measured relative to a hydrogen cell, indicating oxidation or reduction potential}	{salinity measurement}	{none}	{none}	{none}	{concentration of silicate}	{sodium concentration}	{concentration of sulfate}	{concentration of sulfide}	{temperature of the sample at time of sampling}	{total carbon content}	{total nitrogen content of the sample}	{Definition for soil: total organic C content of the soil units of g C/kg soil. Definition otherwise: total organic carbon content}	{turbidity measurement}	{water content measurement}" << endl;
                out << "#{text}	{text}	{text}	{{timestamp}}	{{term}}	{{term}}	{{term}}	{{term}:{term}:{text}}	{{float} {float}}	{{float} m}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{float} m}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{text};{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{term}; {timestamp}}	{{float} {unit}}	{{float} {unit}}	{{text};{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{text};{float} {unit}}	{{text};{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{text};{float} {unit}}	{['', 'aerobic', 'anaerobic']}	{{float} {unit}}	{{text};{interval}}	{{float} {unit}}	{{float} {unit}}	{{text};{float} {unit}}	{{float} {unit}}	{{text};{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{none}}	{{none}}	{{none}}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}" << endl;
                out << "*sample_name	*description	*sample_title	*collection_date	*env_biome	*env_feature	*env_material	*geo_loc_name	*lat_lon	*depth	*elev	alkalinity	alkyl_diethers	altitude	aminopept_act	ammonium	bacteria_carb_prod	biomass	bishomohopanol	bromide	calcium	carb_nitro_ratio	chem_administration	chloride	chlorophyll	diether_lipids	diss_carb_dioxide	diss_hydrogen	diss_inorg_carb	diss_org_carb	diss_org_nitro	diss_oxygen	glucosidase_act	magnesium	mean_frict_vel	mean_peak_frict_vel	methane	misc_param	n_alkanes	nitrate	nitrite	nitro	org_carb	org_matter	org_nitro	organism_count	oxy_stat_samp	part_org_carb	perturbation	petroleum_hydrocarb	ph	phaeopigments	phosphate	phosplipid_fatt_acid	potassium	pressure	redox_potential	salinity	samp_store_dur	samp_store_loc	samp_store_temp	silicate	sodium	sulfate	sulfide	temp	tot_carb	tot_nitro	tot_org_carb	turbidity	water_content" << endl;
            }
        }else if (package == "Miscellaneous") {
            out << "#MIMARKS.survey.miscellaneous.4.0" << endl;
            if (requiredonly) {
                out << "#{sample name}	{description of sample}	{sample title}	{Date of sampling, in \"DD-Mmm-YYYY\", \"Mmm-YYYY\" or \"YYYY\" format (eg., 30-Oct-1990, Oct-1990 or 1990) or ISO 8601 standard \"YYYY-mm-dd\", \"YYYY-mm\" or \"YYYY-mm-ddThh:mm:ss\" (eg., 1990-10-30, 1990-10 or 1990-10-30T14:41:36)}	{descriptor of the broad ecological context of a sample. Examples include: desert, taiga, deciduous woodland, or coral reef. EnvO (v 2013-06-14) terms can be found via the link: www.environmentontology.org/Browse-EnvO}	{descriptor of the local environment. Examples include: harbor, cliff, or lake. EnvO (v 2013-06-14) terms can be found via the link: www.environmentontology.org/Browse-EnvO}	{material that was displaced by the sample, or material in which a sample was embedded, prior to the sampling event. Examples include: air, soil, or water. EnvO (v 2013-06-14) terms can be found via the link: www.environmentontology.org/Browse-EnvO}	{Geographical origin of the sample; use the appropriate name from this list http://www.insdc.org/documents/country-qualifier-vocabulary. Use a colon to separate the country or ocean from more detailed information about the location, eg \"Canada: Vancouver\" or \"Germany: halfway down Zugspitze, Alps\"}	{The geographical coordinates of the location where the sample was collected. Specify as degrees latitude and longitude in format \"d[d.dddd] N|S d[dd.dddd] W|E\", eg, 38.98 N 77.11 W}" << endl;
                out << "#{text}	{text}	{text}	{{timestamp}}	{{term}}	{{term}}	{{term}}	{{term}:{term}:{text}}	{{float} {float}}" << endl;
                out << "*sample_name	*description	*sample_title	*collection_date	*env_biome	*env_feature	*env_material	*geo_loc_name	*lat_lon" << endl;
            }else {
                out << "#{sample name}	{description of sample}	{sample title}	{Date of sampling, in \"DD-Mmm-YYYY\", \"Mmm-YYYY\" or \"YYYY\" format (eg., 30-Oct-1990, Oct-1990 or 1990) or ISO 8601 standard \"YYYY-mm-dd\", \"YYYY-mm\" or \"YYYY-mm-ddThh:mm:ss\" (eg., 1990-10-30, 1990-10 or 1990-10-30T14:41:36)}	{descriptor of the broad ecological context of a sample. Examples include: desert, taiga, deciduous woodland, or coral reef. EnvO (v 2013-06-14) terms can be found via the link: www.environmentontology.org/Browse-EnvO}	{descriptor of the local environment. Examples include: harbor, cliff, or lake. EnvO (v 2013-06-14) terms can be found via the link: www.environmentontology.org/Browse-EnvO}	{material that was displaced by the sample, or material in which a sample was embedded, prior to the sampling event. Examples include: air, soil, or water. EnvO (v 2013-06-14) terms can be found via the link: www.environmentontology.org/Browse-EnvO}	{Geographical origin of the sample; use the appropriate name from this list http://www.insdc.org/documents/country-qualifier-vocabulary. Use a colon to separate the country or ocean from more detailed information about the location, eg \"Canada: Vancouver\" or \"Germany: halfway down Zugspitze, Alps\"}	{The geographical coordinates of the location where the sample was collected. Specify as degrees latitude and longitude in format \"d[d.dddd] N|S d[dd.dddd] W|E\", eg, 38.98 N 77.11 W}	{alkalinity, the ability of a solution to neutralize acids to the equivalence point of carbonate or bicarbonate}	{The altitude of the sample is the vertical distance between Earth's surface above Sea Level and the sampled position in the air.}	{concentration of ammonium}	{amount of biomass; should include the name for the part of biomass measured, e.g. microbial, total. can include multiple measurements}	{concentration of bromide}	{concentration of calcium}	{list of chemical compounds administered to the host or site where sampling occurred, and when (e.g. antibiotics, N fertilizer, air filter); can include multiple compounds. For Chemical Entities of Biological Interest ontology (CHEBI) (v1.72), please see http://bioportal.bioontology.org/visualize/44603}	{concentration of chloride}	{concentration of chlorophyll}	{density of sample}	{Depth is defined as the vertical distance below surface, e.g. for sediment or soil samples depth is measured from sediment or soil surface, respectivly. Depth can be reported as an interval for subsurface samples.}	{concentration of diether lipids; can include multiple types of diether lipids}	{concentration of dissolved carbon dioxide}	{concentration of dissolved hydrogen}	{dissolved inorganic carbon concentration}	{dissolved organic nitrogen concentration measured as; total dissolved nitrogen - NH4 - NO3 - NO2}	{concentration of dissolved oxygen}	{The elevation of the sampling site as measured by the vertical distance from mean sea level.}	{any other measurement performed or parameter collected, that is not listed here}	{concentration of nitrate}	{concentration of nitrite}	{concentration of nitrogen (total)}	{concentration of organic carbon}	{concentration of organic matter}	{concentration of organic nitrogen}	{total count of any organism per gram or volume of sample,should include name of organism followed by count; can include multiple organism counts}	{oxygenation status of sample}	{type of perturbation, e.g. chemical administration, physical disturbance, etc., coupled with time that perturbation occurred; can include multiple perturbation types}	{pH measurement}	{concentration of phosphate}	{concentration of phospholipid fatty acids; can include multiple values}	{concentration of potassium}	{pressure to which the sample is subject, in atmospheres}	{salinity measurement}	{none}	{none}	{none}	{concentration of silicate}	{sodium concentration}	{concentration of sulfate}	{concentration of sulfide}	{temperature of the sample at time of sampling}	{measurement of magnitude and direction of flow within a fluid}" << endl;
                out << "#{text}	{text}	{text}	{{timestamp}}	{{term}}	{{term}}	{{term}}	{{term}:{term}:{text}}	{{float} {float}}	{{float} {unit}}	{{float} m}	{{float} {unit}}	{{text};{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{term}; {timestamp}}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{float} m}	{{text};{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{text};{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{text};{float} {unit}}	{['', 'aerobic', 'anaerobic']}	{{text};{interval}}	{{float} {unit}}	{{float} {unit}}	{{text};{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{none}}	{{none}}	{{none}}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}" << endl;
                out << "*sample_name	*description	*sample_title	*collection_date	*env_biome	*env_feature	*env_material	*geo_loc_name	*lat_lon	alkalinity	altitude	ammonium	biomass	bromide	calcium	chem_administration	chloride	chlorophyll	density	depth	diether_lipids	diss_carb_dioxide	diss_hydrogen	diss_inorg_carb	diss_org_nitro	diss_oxygen	elev	misc_param	nitrate	nitrite	nitro	org_carb	org_matter	org_nitro	organism_count	oxy_stat_samp	perturbation	ph	phosphate	phosplipid_fatt_acid	potassium	pressure	salinity	samp_store_dur	samp_store_loc	samp_store_temp	silicate	sodium	sulfate	sulfide	temp	water_current" << endl;
            }
        }else if (package == "Plant_associated") {
            out << "#MIMARKS.survey.plant-associated.4.0" << endl;
            if (requiredonly) {
                out << "#{sample name}	{description of sample}	{sample title}	{Date of sampling, in \"DD-Mmm-YYYY\", \"Mmm-YYYY\" or \"YYYY\" format (eg., 30-Oct-1990, Oct-1990 or 1990) or ISO 8601 standard \"YYYY-mm-dd\", \"YYYY-mm\" or \"YYYY-mm-ddThh:mm:ss\" (eg., 1990-10-30, 1990-10 or 1990-10-30T14:41:36)}	{descriptor of the broad ecological context of a sample. Examples include: desert, taiga, deciduous woodland, or coral reef. EnvO (v 2013-06-14) terms can be found via the link: www.environmentontology.org/Browse-EnvO}	{descriptor of the local environment. Examples include: harbor, cliff, or lake. EnvO (v 2013-06-14) terms can be found via the link: www.environmentontology.org/Browse-EnvO}	{material that was displaced by the sample, or material in which a sample was embedded, prior to the sampling event. Examples include: air, soil, or water. EnvO (v 2013-06-14) terms can be found via the link: www.environmentontology.org/Browse-EnvO}	{Geographical origin of the sample; use the appropriate name from this list http://www.insdc.org/documents/country-qualifier-vocabulary. Use a colon to separate the country or ocean from more detailed information about the location, eg \"Canada: Vancouver\" or \"Germany: halfway down Zugspitze, Alps\"}	{The geographical coordinates of the location where the sample was collected. Specify as degrees latitude and longitude in format \"d[d.dddd] N|S d[dd.dddd] W|E\", eg, 38.98 N 77.11 W}" << endl;
                out << "#{text}	{text}	{text}	{{timestamp}}	{{term}}	{{term}}	{{term}}	{{term}:{term}:{text}}	{{float} {float}}" << endl;
                out << "*sample_name	*description	*sample_title	*collection_date	*env_biome	*env_feature	*env_material	*geo_loc_name	*lat_lon" << endl;
            }else {
                out << "#{sample name}	{description of sample}	{sample title}	{Date of sampling, in \"DD-Mmm-YYYY\", \"Mmm-YYYY\" or \"YYYY\" format (eg., 30-Oct-1990, Oct-1990 or 1990) or ISO 8601 standard \"YYYY-mm-dd\", \"YYYY-mm\" or \"YYYY-mm-ddThh:mm:ss\" (eg., 1990-10-30, 1990-10 or 1990-10-30T14:41:36)}	{descriptor of the broad ecological context of a sample. Examples include: desert, taiga, deciduous woodland, or coral reef. EnvO (v 2013-06-14) terms can be found via the link: www.environmentontology.org/Browse-EnvO}	{descriptor of the local environment. Examples include: harbor, cliff, or lake. EnvO (v 2013-06-14) terms can be found via the link: www.environmentontology.org/Browse-EnvO}	{material that was displaced by the sample, or material in which a sample was embedded, prior to the sampling event. Examples include: air, soil, or water. EnvO (v 2013-06-14) terms can be found via the link: www.environmentontology.org/Browse-EnvO}	{Geographical origin of the sample; use the appropriate name from this list http://www.insdc.org/documents/country-qualifier-vocabulary. Use a colon to separate the country or ocean from more detailed information about the location, eg \"Canada: Vancouver\" or \"Germany: halfway down Zugspitze, Alps\"}	{The geographical coordinates of the location where the sample was collected. Specify as degrees latitude and longitude in format \"d[d.dddd] N|S d[dd.dddd] W|E\", eg, 38.98 N 77.11 W}	{information about treatment involving an exposure to varying temperatures; should include the temperature, treatment duration, interval and total experimental duration; can include different temperature regimens}	{The altitude of the sample is the vertical distance between Earth's surface above Sea Level and the sampled position in the air.}	{information about treatment involving antibiotic administration; should include the name of antibiotic, amount administered, treatment duration, interval and total experimental duration; can include multiple antibiotic regimens}	{list of chemical compounds administered to the host or site where sampling occurred, and when (e.g. antibiotics, N fertilizer, air filter); can include multiple compounds. For Chemical Entities of Biological Interest ontology (CHEBI) (v1.72), please see http://bioportal.bioontology.org/visualize/44603}	{treatment involving use of mutagens; should include the name of mutagen, amount administered, treatment duration, interval and total experimental duration; can include multiple mutagen regimens}	{treatment involving an exposure to a particular climate; can include multiple climates}	{Depth is defined as the vertical distance below surface, e.g. for sediment or soil samples depth is measured from sediment or soil surface, respectivly. Depth can be reported as an interval for subsurface samples.}	{The elevation of the sampling site as measured by the vertical distance from mean sea level.}	{information about treatment involving the use of fertilizers; should include the name fertilizer, amount administered, treatment duration, interval and total experimental duration; can include multiple fertilizer regimens}	{information about treatment involving use of fungicides; should include the name of fungicide, amount administered, treatment duration, interval and total experimental duration; can include multiple fungicide regimens}	{use of conditions with differing gaseous environments; should include the name of gaseous compound, amount administered, treatment duration, interval and total experimental duration; can include multiple gaseous environment regimens}	{information about treatment involving use of gravity factor to study various types of responses in presence, absence or modified levels of gravity; can include multiple treatments}	{information about treatment involving use of growth hormones; should include the name of growth hormone, amount administered, treatment duration, interval and total experimental duration; can include multiple growth hormone regimens}	{information about growth media for growing the plants or tissue cultured samples}	{information about treatment involving use of herbicides; information about treatment involving use of growth hormones; should include the name of herbicide, amount administered, treatment duration, interval and total experimental duration; can include multiple regimens}	{Age of host at the time of sampling}	{Name of relevant disease, e.g. Salmonella gastroenteritis. Controlled vocabulary, http://bioportal.bioontology.org/ontologies/1009 or http://www.ncbi.nlm.nih.gov/mesh}	{measurement of dry mass}	{none}	{the height of subject}	{taxonomic information subspecies level}	{taxonomic rank information below subspecies level, such as variety, form, rank etc.}	{the length of subject}	{description of host life stage}	{none}	{NCBI taxonomy ID of the host, e.g. 9606}	{total mass of the host at collection, the unit depends on host}	{measurement of wet mass}	{information about treatment involving an exposure to varying degree of humidity; information about treatment involving use of growth hormones; should include amount of humidity administered, treatment duration, interval and total experimental duration; can include multiple regimens}	{information about any mechanical damage exerted on the plant; can include multiple damages and sites}	{information about treatment involving the use of mineral supplements; should include the name of mineral nutrient, amount administered, treatment duration, interval and total experimental duration; can include multiple mineral nutrient regimens}	{any other measurement performed or parameter collected, that is not listed here}	{information about treatment involving the exposure of plant to non-mineral nutrient such as oxygen, hydrogen or carbon; should include the name of non-mineral nutrient, amount administered, treatment duration, interval and total experimental duration; can include multiple non-mineral nutrient regimens}	{total count of any organism per gram or volume of sample,should include name of organism followed by count; can include multiple organism counts}	{oxygenation status of sample}	{type of perturbation, e.g. chemical administration, physical disturbance, etc., coupled with time that perturbation occurred; can include multiple perturbation types}	{information about treatment involving use of insecticides; should include the name of pesticide, amount administered, treatment duration, interval and total experimental duration; can include multiple pesticide regimens}	{information about treatment involving exposure of plants to varying levels of pH of the growth media; can include multiple regimen}	{name of body site that the sample was obtained from. For Plant Ontology (PO) (v 20) terms, see http://purl.bioontology.org/ontology/PO}	{substance produced by the plant, where the sample was obtained from}	{information about treatment involving exposure of plant or a plant part to a particular radiation regimen; should include the radiation type, amount or intensity administered, treatment duration, interval and total experimental duration; can include multiple radiation regimens}	{information about treatment involving an exposure to a given amount of rainfall; can include multiple regimens}	{information about treatment involving use of salts as supplement to liquid and soil growth media; should include the name of salt, amount administered, treatment duration, interval and total experimental duration; can include multiple salt regimens}	{none}	{none}	{none}	{none}	{treatment involving an exposure to a particular season (e.g. winter, summer, rabi, rainy etc.)}	{treatment involving an exposure to standing water during a plant's life span, types can be flood water or standing water; can include multiple regimens}	{temperature of the sample at time of sampling}	{description of plant tissue culture growth media used}	{information about treatment involving an exposure to water with varying degree of temperature; can include multiple regimens}	{information about treatment involving an exposure to watering frequencies; can include multiple regimens}" << endl;
                out << "#{text}	{text}	{text}	{{timestamp}}	{{term}}	{{term}}	{{term}}	{{term}:{term}:{text}}	{{float} {float}}	{{float} {unit};{period};{interval};{period}}	{{float} m}	{{text};{float} {unit};{period};{interval};{period}}	{{term}; {timestamp}}	{{text};{float} {unit};{period};{interval};{period}}	{{text};{period};{interval};{period}}	{{float} m}	{{float} {unit}}	{{text};{float} {unit};{period};{interval};{period}}	{{text};{float} {unit};{period};{interval};{period}}	{{text};{float} {unit};{period};{interval};{period}}	{{float} {unit};{period};{interval};{period}}	{{text};{float} {unit};{period};{interval};{period}}	{['', 'soil', 'liquid']}	{{text};{float} {unit};{period};{interval};{period}}	{{none}}	{{none}}	{{float} {unit}}	{{none}}	{{float} {unit}}	{{text}}	{{text}}	{{float} {unit}}	{{text}}	{{none}}	{{integer}}	{{float} {unit}}	{{float} {unit}}	{{float} {unit};{period};{interval};{period}}	{{text};{text}}	{{text};{float} {unit};{period};{interval};{period}}	{{text};{float} {unit}}	{{text};{float} {unit};{period};{interval};{period}}	{{text};{float} {unit}}	{['', 'aerobic', 'anaerobic']}	{{text};{interval}}	{{text};{float} {unit};{period};{interval};{period}}	{{float} {unit};{period};{interval};{period}}	{{term}}	{{text}}	{{text};{float} {unit};{period};{interval};{period}}	{{float} {unit};{period};{interval};{period}}	{{text};{float} {unit};{period};{interval};{period}}	{{none}}	{{none}}	{{none}}	{{none}}	{{text};{period};{interval};{period}}	{{text};{period};{interval};{period}}	{{float} {unit}}	{{PMID|DOI|URL}}	{{float} {unit};{period};{interval};{period}}	{{float} {unit};{period};{interval};{period}}" << endl;
                out << "*sample_name	*description	*sample_title	*collection_date	*env_biome	*env_feature	*env_material	*geo_loc_name	*lat_lon	air_temp_regm	altitude	antibiotic_regm	chem_administration	chem_mutagen	climate_environment	depth	elev	fertilizer_regm	fungicide_regm	gaseous_environment	gravity	growth_hormone_regm	growth_med	herbicide_regm	host_age	host_disease	host_dry_mass	host_genotype	host_height	host_infra_specific_name	host_infra_specific_rank	host_length	host_life_stage	host_phenotype	host_taxid	host_tot_mass	host_wet_mass	humidity_regm	mechanical_damage	mineral_nutr_regm	misc_param	non_mineral_nutr_regm	organism_count	oxy_stat_samp	perturbation	pesticide_regm	ph_regm	plant_body_site	plant_product	radiation_regm	rainfall_regm	salt_regm	samp_salinity	samp_store_dur	samp_store_loc	samp_store_temp	season_environment	standing_water_regm	temp	tiss_cult_growth_med	water_temp_regm	watering_regm" << endl;
            }
        }else if (package == "Sediment") {
            out << "#MIMARKS.survey.sediment.4.0" << endl;
            if (requiredonly) {
                out << "#{sample name}	{description of sample}	{sample title}	{Date of sampling, in \"DD-Mmm-YYYY\", \"Mmm-YYYY\" or \"YYYY\" format (eg., 30-Oct-1990, Oct-1990 or 1990) or ISO 8601 standard \"YYYY-mm-dd\", \"YYYY-mm\" or \"YYYY-mm-ddThh:mm:ss\" (eg., 1990-10-30, 1990-10 or 1990-10-30T14:41:36)}	{descriptor of the broad ecological context of a sample. Examples include: desert, taiga, deciduous woodland, or coral reef. EnvO (v 2013-06-14) terms can be found via the link: www.environmentontology.org/Browse-EnvO}	{descriptor of the local environment. Examples include: harbor, cliff, or lake. EnvO (v 2013-06-14) terms can be found via the link: www.environmentontology.org/Browse-EnvO}	{material that was displaced by the sample, or material in which a sample was embedded, prior to the sampling event. Examples include: air, soil, or water. EnvO (v 2013-06-14) terms can be found via the link: www.environmentontology.org/Browse-EnvO}	{Geographical origin of the sample; use the appropriate name from this list http://www.insdc.org/documents/country-qualifier-vocabulary. Use a colon to separate the country or ocean from more detailed information about the location, eg \"Canada: Vancouver\" or \"Germany: halfway down Zugspitze, Alps\"}	{The geographical coordinates of the location where the sample was collected. Specify as degrees latitude and longitude in format \"d[d.dddd] N|S d[dd.dddd] W|E\", eg, 38.98 N 77.11 W}	{Depth is defined as the vertical distance below surface, e.g. for sediment or soil samples depth is measured from sediment or soil surface, respectivly. Depth can be reported as an interval for subsurface samples.}	{The elevation of the sampling site as measured by the vertical distance from mean sea level.}" << endl;
                out << "#{text}	{text}	{text}	{{timestamp}}	{{term}}	{{term}}	{{term}}	{{term}:{term}:{text}}	{{float} {float}}	{{float} m}	{{float} {unit}}" << endl;
                out << "*sample_name	*description	*sample_title	*collection_date	*env_biome	*env_feature	*env_material	*geo_loc_name	*lat_lon	*depth	*elev" << endl;
            }else {
                out << "#{sample name}	{description of sample}	{sample title}	{Date of sampling, in \"DD-Mmm-YYYY\", \"Mmm-YYYY\" or \"YYYY\" format (eg., 30-Oct-1990, Oct-1990 or 1990) or ISO 8601 standard \"YYYY-mm-dd\", \"YYYY-mm\" or \"YYYY-mm-ddThh:mm:ss\" (eg., 1990-10-30, 1990-10 or 1990-10-30T14:41:36)}	{descriptor of the broad ecological context of a sample. Examples include: desert, taiga, deciduous woodland, or coral reef. EnvO (v 2013-06-14) terms can be found via the link: www.environmentontology.org/Browse-EnvO}	{descriptor of the local environment. Examples include: harbor, cliff, or lake. EnvO (v 2013-06-14) terms can be found via the link: www.environmentontology.org/Browse-EnvO}	{material that was displaced by the sample, or material in which a sample was embedded, prior to the sampling event. Examples include: air, soil, or water. EnvO (v 2013-06-14) terms can be found via the link: www.environmentontology.org/Browse-EnvO}	{Geographical origin of the sample; use the appropriate name from this list http://www.insdc.org/documents/country-qualifier-vocabulary. Use a colon to separate the country or ocean from more detailed information about the location, eg \"Canada: Vancouver\" or \"Germany: halfway down Zugspitze, Alps\"}	{The geographical coordinates of the location where the sample was collected. Specify as degrees latitude and longitude in format \"d[d.dddd] N|S d[dd.dddd] W|E\", eg, 38.98 N 77.11 W}	{Depth is defined as the vertical distance below surface, e.g. for sediment or soil samples depth is measured from sediment or soil surface, respectivly. Depth can be reported as an interval for subsurface samples.}	{The elevation of the sampling site as measured by the vertical distance from mean sea level.}	{alkalinity, the ability of a solution to neutralize acids to the equivalence point of carbonate or bicarbonate}	{concentration of alkyl diethers}	{The altitude of the sample is the vertical distance between Earth's surface above Sea Level and the sampled position in the air.}	{measurement of aminopeptidase activity}	{concentration of ammonium}	{measurement of bacterial carbon production}	{amount of biomass; should include the name for the part of biomass measured, e.g. microbial, total. can include multiple measurements}	{concentration of bishomohopanol}	{concentration of bromide}	{concentration of calcium}	{ratio of amount or concentrations of carbon to nitrogen}	{list of chemical compounds administered to the host or site where sampling occurred, and when (e.g. antibiotics, N fertilizer, air filter); can include multiple compounds. For Chemical Entities of Biological Interest ontology (CHEBI) (v1.72), please see http://bioportal.bioontology.org/visualize/44603}	{concentration of chloride}	{concentration of chlorophyll}	{density of sample}	{concentration of diether lipids; can include multiple types of diether lipids}	{concentration of dissolved carbon dioxide}	{concentration of dissolved hydrogen}	{dissolved inorganic carbon concentration}	{concentration of dissolved organic carbon}	{dissolved organic nitrogen concentration measured as; total dissolved nitrogen - NH4 - NO3 - NO2}	{concentration of dissolved oxygen}	{measurement of glucosidase activity}	{concentration of magnesium}	{measurement of mean friction velocity}	{measurement of mean peak friction velocity}	{methane (gas) amount or concentration at the time of sampling}	{any other measurement performed or parameter collected, that is not listed here}	{concentration of n-alkanes; can include multiple n-alkanes}	{concentration of nitrate}	{concentration of nitrite}	{concentration of nitrogen (total)}	{concentration of organic carbon}	{concentration of organic matter}	{concentration of organic nitrogen}	{total count of any organism per gram or volume of sample,should include name of organism followed by count; can include multiple organism counts}	{oxygenation status of sample}	{concentration of particulate organic carbon}	{particles are classified, based on their size, into six general categories:clay, silt, sand, gravel, cobbles, and boulders; should include amount of particle preceded by the name of the particle type; can include multiple values}	{type of perturbation, e.g. chemical administration, physical disturbance, etc., coupled with time that perturbation occurred; can include multiple perturbation types}	{concentration of petroleum hydrocarbon}	{pH measurement}	{concentration of phaeopigments; can include multiple phaeopigments}	{concentration of phosphate}	{concentration of phospholipid fatty acids; can include multiple values}	{porosity of deposited sediment is volume of voids divided by the total volume of sample}	{concentration of potassium}	{pressure to which the sample is subject, in atmospheres}	{redox potential, measured relative to a hydrogen cell, indicating oxidation or reduction potential}	{salinity measurement}	{none}	{none}	{none}	{information about the sediment type based on major constituents}	{concentration of silicate}	{sodium concentration}	{concentration of sulfate}	{concentration of sulfide}	{temperature of the sample at time of sampling}	{stage of tide}	{total carbon content}	{total nitrogen content of the sample}	{Definition for soil: total organic C content of the soil units of g C/kg soil. Definition otherwise: total organic carbon content}	{turbidity measurement}	{water content measurement}" << endl;
                out << "#{text}	{text}	{text}	{{timestamp}}	{{term}}	{{term}}	{{term}}	{{term}:{term}:{text}}	{{float} {float}}	{{float} m}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{float} m}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{text};{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{term}; {timestamp}}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{text};{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{text};{float} {unit}}	{{text};{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{text};{float} {unit}}	{['', 'aerobic', 'anaerobic']}	{{float} {unit}}	{{text};{float} {unit}}	{{text};{interval}}	{{float} {unit}}	{{float} {unit}}	{{text};{float} {unit}}	{{float} {unit}}	{{text};{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{none}}	{{none}}	{{none}}	{['', 'biogenous', 'cosmogenous', 'hydrogenous', 'lithogenous']}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{['', 'low', 'high']}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}" << endl;
                out << "*sample_name	*description	*sample_title	*collection_date	*env_biome	*env_feature	*env_material	*geo_loc_name	*lat_lon	*depth	*elev	alkalinity	alkyl_diethers	altitude	aminopept_act	ammonium	bacteria_carb_prod	biomass	bishomohopanol	bromide	calcium	carb_nitro_ratio	chem_administration	chloride	chlorophyll	density	diether_lipids	diss_carb_dioxide	diss_hydrogen	diss_inorg_carb	diss_org_carb	diss_org_nitro	diss_oxygen	glucosidase_act	magnesium	mean_frict_vel	mean_peak_frict_vel	methane	misc_param	n_alkanes	nitrate	nitrite	nitro	org_carb	org_matter	org_nitro	organism_count	oxy_stat_samp	part_org_carb	particle_class	perturbation	petroleum_hydrocarb	ph	phaeopigments	phosphate	phosplipid_fatt_acid	porosity	potassium	pressure	redox_potential	salinity	samp_store_dur	samp_store_loc	samp_store_temp	sediment_type	silicate	sodium	sulfate	sulfide	temp	tidal_stage	tot_carb	tot_nitro	tot_org_carb	turbidity	water_content" << endl;
            }
        }else if (package == "Soil") {
            out << "#MIMARKS.survey.soil.4.0" << endl;
            if (requiredonly) {
                out << "#{sample name}	{description of sample}	{sample title}	{Date of sampling, in \"DD-Mmm-YYYY\", \"Mmm-YYYY\" or \"YYYY\" format (eg., 30-Oct-1990, Oct-1990 or 1990) or ISO 8601 standard \"YYYY-mm-dd\", \"YYYY-mm\" or \"YYYY-mm-ddThh:mm:ss\" (eg., 1990-10-30, 1990-10 or 1990-10-30T14:41:36)}	{descriptor of the broad ecological context of a sample. Examples include: desert, taiga, deciduous woodland, or coral reef. EnvO (v 2013-06-14) terms can be found via the link: www.environmentontology.org/Browse-EnvO}	{descriptor of the local environment. Examples include: harbor, cliff, or lake. EnvO (v 2013-06-14) terms can be found via the link: www.environmentontology.org/Browse-EnvO}	{material that was displaced by the sample, or material in which a sample was embedded, prior to the sampling event. Examples include: air, soil, or water. EnvO (v 2013-06-14) terms can be found via the link: www.environmentontology.org/Browse-EnvO}	{Geographical origin of the sample; use the appropriate name from this list http://www.insdc.org/documents/country-qualifier-vocabulary. Use a colon to separate the country or ocean from more detailed information about the location, eg \"Canada: Vancouver\" or \"Germany: halfway down Zugspitze, Alps\"}	{The geographical coordinates of the location where the sample was collected. Specify as degrees latitude and longitude in format \"d[d.dddd] N|S d[dd.dddd] W|E\", eg, 38.98 N 77.11 W}	{Depth is defined as the vertical distance below surface, e.g. for sediment or soil samples depth is measured from sediment or soil surface, respectivly. Depth can be reported as an interval for subsurface samples.}	{The elevation of the sampling site as measured by the vertical distance from mean sea level.}" << endl;
                out << "#{text}	{text}	{text}	{{timestamp}}	{{term}}	{{term}}	{{term}}	{{term}:{term}:{text}}	{{float} {float}}	{{float} m}	{{float} {unit}}" << endl;
                out << "*sample_name	*description	*sample_title	*collection_date	*env_biome	*env_feature	*env_material	*geo_loc_name	*lat_lon	*depth	*elev" << endl;
            }else {
                out << "#{sample name}	{description of sample}	{sample title}	{Date of sampling, in \"DD-Mmm-YYYY\", \"Mmm-YYYY\" or \"YYYY\" format (eg., 30-Oct-1990, Oct-1990 or 1990) or ISO 8601 standard \"YYYY-mm-dd\", \"YYYY-mm\" or \"YYYY-mm-ddThh:mm:ss\" (eg., 1990-10-30, 1990-10 or 1990-10-30T14:41:36)}	{descriptor of the broad ecological context of a sample. Examples include: desert, taiga, deciduous woodland, or coral reef. EnvO (v 2013-06-14) terms can be found via the link: www.environmentontology.org/Browse-EnvO}	{descriptor of the local environment. Examples include: harbor, cliff, or lake. EnvO (v 2013-06-14) terms can be found via the link: www.environmentontology.org/Browse-EnvO}	{material that was displaced by the sample, or material in which a sample was embedded, prior to the sampling event. Examples include: air, soil, or water. EnvO (v 2013-06-14) terms can be found via the link: www.environmentontology.org/Browse-EnvO}	{Geographical origin of the sample; use the appropriate name from this list http://www.insdc.org/documents/country-qualifier-vocabulary. Use a colon to separate the country or ocean from more detailed information about the location, eg \"Canada: Vancouver\" or \"Germany: halfway down Zugspitze, Alps\"}	{The geographical coordinates of the location where the sample was collected. Specify as degrees latitude and longitude in format \"d[d.dddd] N|S d[dd.dddd] W|E\", eg, 38.98 N 77.11 W}	{Depth is defined as the vertical distance below surface, e.g. for sediment or soil samples depth is measured from sediment or soil surface, respectivly. Depth can be reported as an interval for subsurface samples.}	{The elevation of the sampling site as measured by the vertical distance from mean sea level.}	{addition of fertilizers, pesticides, etc. - amount and time of applications}	{aluminum saturation (esp. for tropical soils)}	{reference or method used in determining Al saturation}	{The altitude of the sample is the vertical distance between Earth's surface above Sea Level and the sampled position in the air.}	{mean annual and seasonal precipitation (mm)}	{mean annual and seasonal temperature (oC)}	{whether or not crop is rotated, and if yes, rotation schedule}	{present state of sample site}	{vegetation classification from one or more standard classification systems, or agricultural crop}	{reference or method used in vegetation classification}	{drainage classification from a standard system such as the USDA system}	{unusual physical events that may have affected microbial populations}	{measured salinity}	{soil classification from the FAO World Reference Database for Soil Resources}	{historical and/or physical evidence of fire}	{historical and/or physical evidence of flooding}	{heavy metals present and concentrationsany drug used by subject and the frequency of usage; can include multiple heavy metals and concentrations}	{reference or method used in determining heavy metals}	{specific layer in the land area which measures parallel to the soil surface and possesses physical characteristics which differ from the layers above and beneath}	{reference or method used in determining the horizon}	{none}	{link to digitized soil maps or other soil classification information}	{link to climate resource}	{soil classification based on local soil classification system}	{reference or method used in determining the local soil classification}	{the part of the organic matter in the soil that constitutes living microorganisms smaller than 5-10 µm. IF you keep this, you would need to have correction factors used for conversion to the final units, which should be mg C (or N)/kg soil).}	{reference or method used in determining microbial biomass}	{any other measurement performed or parameter collected, that is not listed here}	{pH measurement}	{reference or method used in determining pH}	{were multiple DNA extractions mixed? how many?}	{previous land use and dates}	{reference or method used in determining previous land use and dates}	{cross-sectional position in the hillslope where sample was collected.sample area position in relation to surrounding areas}	{reference or method used in determining salinity}	{collection design of pooled samples and/or sieve size and amount of sample sieved}	{the direction a slope faces. While looking down a slope use a compass to record the direction you are facing (direction or degrees); e.g., NW or 315°. This measure provides an indication of sun and wind exposure that will influence soil temperature and evapotranspiration.}	{commonly called “slope.” The angle between ground surface and a horizontal line (in percent). This is the direction that overland water would flow. This measure is usually taken with a hand level meter or clinometer.}	{soil series name or other lower-level classification}	{reference or method used in determining soil series name or other lower-level classification}	{explain how and for how long the soil sample was stored before DNA extraction.}	{the relative proportion of different grain sizes of mineral particles in a soil, as described using a standard system; express as % sand (50 um to 2 mm), silt (2 um to 50 um), and clay (}	{reference or method used in determining soil texture}	{note method(s) used for tilling}	{reference or method used in determining the total N}	{total nitrogen content of the sample}	{reference or method used in determining total organic C}	{Definition for soil: total organic C content of the soil units of g C/kg soil. Definition otherwise: total organic carbon content}	{water content (g/g or cm3/cm3)}	{reference or method used in determining the water content of soil}" << endl;
                out << "#{text}	{text}	{text}	{{timestamp}}	{{term}}	{{term}}	{{term}}	{{term}:{term}:{text}}	{{float} {float}}	{{float} m}	{{float} {unit}}	{{text};{float} {unit};{timestamp}}	{{float} {unit}}	{{PMID|DOI|URL}}	{{float} m}	{{float} {unit}}	{{float} {unit}}	{{boolean};Rn/{timestamp}/{period}}	{['', 'cities', 'farmstead', 'industrial areas', 'roads/railroads', 'rock', 'sand', 'gravel', 'mudflats', 'salt flats', 'badlands', 'permanent snow or ice', 'saline seeps', 'mines/quarries', 'oil waste areas', 'small grains', 'row crops', 'vegetable crops', 'horticultural plants (e.g. tulips)', 'marshlands (grass,sedges,rushes)', 'tundra (mosses,lichens)', 'rangeland', 'pastureland (grasslands used for livestock grazing)', 'hayland', 'meadows (grasses,alfalfa,fescue,bromegrass,timothy)', 'shrub land (e.g. mesquite,sage-brush,creosote bush,shrub oak,eucalyptus)', 'successional shrub land (tree saplings,hazels,sumacs,chokecherry,shrub dogwoods,blackberries)', 'shrub crops (blueberries,nursery ornamentals,filberts)', 'vine crops (grapes)', 'conifers (e.g. pine,spruce,fir,cypress)', 'hardwoods (e.g. oak,hickory,elm,aspen)', 'intermixed hardwood and conifers', 'tropical (e.g. mangrove,palms)', 'rainforest (evergreen forest receiving <}	{{text}}	{{PMID|DOI|URL}}	{['', 'very poorly', 'poorly', 'somewhat poorly', 'moderately well', 'well', 'excessively drained']}	{{timestamp}}	{{float} {unit}}	{{term}}	{{timestamp}}	{{timestamp}}	{{text};{float} {unit}}	{{PMID|DOI|URL}}	{['', 'O horizon', 'A horizon', 'E horizon', 'B horizon', 'C horizon', 'R layer', 'Permafrost']}	{{PMID|DOI|URL}}	{{PMID|DOI|URL}}	{{PMID|DOI|URL}}	{{PMID|DOI|URL}}	{{text}}	{{PMID|DOI|URL}}	{{float} {unit}}	{{PMID|DOI|URL}}	{{text};{float} {unit}}	{{float} {unit}}	{{PMID|DOI|URL}}	{{boolean};{float} {unit}}	{{text};{timestamp}}	{{PMID|DOI|URL}}	{['', 'summit', 'shoulder', 'backslope', 'footslope', 'toeslope']}	{{PMID|DOI|URL}}	{{{text}|{float} {unit}};{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{text}}	{{PMID|DOI|URL}}	{{text};{period}}	{{float} {unit}}	{{PMID|DOI|URL}}	{['', 'drill', 'cutting disc', 'ridge till', 'strip tillage', 'zonal tillage', 'chisel', 'tined', 'mouldboard', 'disc plough']}	{{PMID|DOI|URL}}	{{float} {unit}}	{{PMID|DOI|URL}}	{{float} {unit}}	{{float} [g/g|cm3/cm3]}	{{PMID|DOI|URL}}" << endl;
                out << "*sample_name	*description	*sample_title	*collection_date	*env_biome	*env_feature	*env_material	*geo_loc_name	*lat_lon	*depth	*elev	agrochem_addition	al_sat	al_sat_meth	altitude	annual_season_precpt	annual_season_temp	crop_rotation	cur_land_use	cur_vegetation	cur_vegetation_meth	drainage_class	extreme_event	extreme_salinity	fao_class	fire	flooding	heavy_metals	heavy_metals_meth	horizon	horizon_meth	link_addit_analys	link_class_info	link_climate_info	local_class	local_class_meth	microbial_biomass	microbial_biomass_meth	misc_param	ph	ph_meth	pool_dna_extracts	previous_land_use	previous_land_use_meth	profile_position	salinity_meth	sieving	slope_aspect	slope_gradient	soil_type	soil_type_meth	store_cond	texture	texture_meth	tillage	tot_n_meth	tot_nitro	tot_org_c_meth	tot_org_carb	water_content_soil	water_content_soil_meth" << endl;
            }
        }else if (package == "Wastewater") {
            out << "#MIMARKS.survey.wastewater.4.0" << endl;
            if (requiredonly) {
                out << "#{sample name}	{description of sample}	{sample title}	{Date of sampling, in \"DD-Mmm-YYYY\", \"Mmm-YYYY\" or \"YYYY\" format (eg., 30-Oct-1990, Oct-1990 or 1990) or ISO 8601 standard \"YYYY-mm-dd\", \"YYYY-mm\" or \"YYYY-mm-ddThh:mm:ss\" (eg., 1990-10-30, 1990-10 or 1990-10-30T14:41:36)}	{descriptor of the broad ecological context of a sample. Examples include: desert, taiga, deciduous woodland, or coral reef. EnvO (v 2013-06-14) terms can be found via the link: www.environmentontology.org/Browse-EnvO}	{descriptor of the local environment. Examples include: harbor, cliff, or lake. EnvO (v 2013-06-14) terms can be found via the link: www.environmentontology.org/Browse-EnvO}	{material that was displaced by the sample, or material in which a sample was embedded, prior to the sampling event. Examples include: air, soil, or water. EnvO (v 2013-06-14) terms can be found via the link: www.environmentontology.org/Browse-EnvO}	{Geographical origin of the sample; use the appropriate name from this list http://www.insdc.org/documents/country-qualifier-vocabulary. Use a colon to separate the country or ocean from more detailed information about the location, eg \"Canada: Vancouver\" or \"Germany: halfway down Zugspitze, Alps\"}	{The geographical coordinates of the location where the sample was collected. Specify as degrees latitude and longitude in format \"d[d.dddd] N|S d[dd.dddd] W|E\", eg, 38.98 N 77.11 W}" << endl;
                out << "#{text}	{text}	{text}	{{timestamp}}	{{term}}	{{term}}	{{term}}	{{term}:{term}:{text}}	{{float} {float}}" << endl;
                out << "*sample_name	*description	*sample_title	*collection_date	*env_biome	*env_feature	*env_material	*geo_loc_name	*lat_lon" << endl;
            }else {
                out << "#{sample name}	{description of sample}	{sample title}	{Date of sampling, in \"DD-Mmm-YYYY\", \"Mmm-YYYY\" or \"YYYY\" format (eg., 30-Oct-1990, Oct-1990 or 1990) or ISO 8601 standard \"YYYY-mm-dd\", \"YYYY-mm\" or \"YYYY-mm-ddThh:mm:ss\" (eg., 1990-10-30, 1990-10 or 1990-10-30T14:41:36)}	{descriptor of the broad ecological context of a sample. Examples include: desert, taiga, deciduous woodland, or coral reef. EnvO (v 2013-06-14) terms can be found via the link: www.environmentontology.org/Browse-EnvO}	{descriptor of the local environment. Examples include: harbor, cliff, or lake. EnvO (v 2013-06-14) terms can be found via the link: www.environmentontology.org/Browse-EnvO}	{material that was displaced by the sample, or material in which a sample was embedded, prior to the sampling event. Examples include: air, soil, or water. EnvO (v 2013-06-14) terms can be found via the link: www.environmentontology.org/Browse-EnvO}	{Geographical origin of the sample; use the appropriate name from this list http://www.insdc.org/documents/country-qualifier-vocabulary. Use a colon to separate the country or ocean from more detailed information about the location, eg \"Canada: Vancouver\" or \"Germany: halfway down Zugspitze, Alps\"}	{The geographical coordinates of the location where the sample was collected. Specify as degrees latitude and longitude in format \"d[d.dddd] N|S d[dd.dddd] W|E\", eg, 38.98 N 77.11 W}	{alkalinity, the ability of a solution to neutralize acids to the equivalence point of carbonate or bicarbonate}	{a measure of the relative oxygen-depletion effect of a waste contaminant}	{list of chemical compounds administered to the host or site where sampling occurred, and when (e.g. antibiotics, N fertilizer, air filter); can include multiple compounds. For Chemical Entities of Biological Interest ontology (CHEBI) (v1.72), please see http://bioportal.bioontology.org/visualize/44603}	{a measure of the relative oxygen-depletion effect of a waste contaminant}	{Depth is defined as the vertical distance below surface, e.g. for sediment or soil samples depth is measured from sediment or soil surface, respectivly. Depth can be reported as an interval for subsurface samples.}	{percentage of volatile solids removed from the anaerobic digestor}	{amount or concentration of substances such as paints, adhesives, mayonnaise, hair colorants, emulsified oils, etc.; can include multiple emulsion types}	{amount or concentration of substances such as hydrogen sulfide, carbon dioxide, methane, etc.; can include multiple substances}	{percentage of industrial effluents received by wastewater treatment plant}	{concentration of particles such as sand, grit, metal particles, ceramics, etc.; can include multiple particles}	{any other measurement performed or parameter collected, that is not listed here}	{concentration of nitrate}	{concentration of particles such as faeces, hairs, food, vomit, paper fibers, plant material, humus, etc.}	{total count of any organism per gram or volume of sample,should include name of organism followed by count; can include multiple organism counts}	{oxygenation status of sample}	{type of perturbation, e.g. chemical administration, physical disturbance, etc., coupled with time that perturbation occurred; can include multiple perturbation types}	{pH measurement}	{concentration of phosphate}	{the process of pre-treatment removes materials that can be easily collected from the raw wastewater}	{the process to produce both a generally homogeneous liquid capable of being treated biologically and a sludge that can be separately treated or processed}	{anaerobic digesters can be designed and engineered to operate using a number of different process configurations, as batch or continuous, mesophilic, high solid or low solid, and single stage or multistage}	{none}	{none}	{none}	{none}	{the process for substantially degrading the biological content of the sewage}	{type of wastewater treatment plant as municipial or industrial}	{the time activated sludge remains in reactor}	{sodium concentration}	{concentration of substances such as ammonia, road-salt, sea-salt, cyanide, hydrogen sulfide, thiocyanates, thiosulfates, etc.}	{concentration of substances such as urea, fruit sugars, soluble proteins, drugs, pharmaceuticals, etc.}	{concentration of substances including a wide variety of material, such as silt, decaying plant and animal matter, etc,; can include multiple substances}	{temperature of the sample at time of sampling}	{the process providing a final treatment stage to raise the effluent quality before it is discharged to the receiving environment}	{total nitrogen content of the sample}	{total amount or concentration of phosphate}	{the origin of wastewater such as human waste, rainfall, storm drains, etc.}" << endl;
                out << "#{text}	{text}	{text}	{{timestamp}}	{{term}}	{{term}}	{{term}}	{{term}:{term}:{text}}	{{float} {float}}	{{float} {unit}}	{{float} {unit}}	{{term}; {timestamp}}	{{float} {unit}}	{{float} m}	{{float} {unit}}	{{text};{float} {unit}}	{{text};{float} {unit}}	{{float} {unit}}	{{text};{float} {unit}}	{{text};{float} {unit}}	{{float} {unit}}	{{text};{float} {unit}}	{{text};{float} {unit}}	{['', 'aerobic', 'anaerobic']}	{{text};{interval}}	{{float} {unit}}	{{float} {unit}}	{{text}}	{{text}}	{{text}}	{{none}}	{{none}}	{{none}}	{{none}}	{{text}}	{{text}}	{{float} {unit}}	{{float} {unit}}	{{text};{float} {unit}}	{{text};{float} {unit}}	{{text};{float} {unit}}	{{float} {unit}}	{{text}}	{{float} {unit}}	{{float} {unit}}	{{text}}" << endl;
                out << "*sample_name	*description	*sample_title	*collection_date	*env_biome	*env_feature	*env_material	*geo_loc_name	*lat_lon	alkalinity	biochem_oxygen_dem	chem_administration	chem_oxygen_dem	depth	efficiency_percent	emulsions	gaseous_substances	indust_eff_percent	inorg_particles	misc_param	nitrate	org_particles	organism_count	oxy_stat_samp	perturbation	ph	phosphate	pre_treatment	primary_treatment	reactor_type	samp_salinity	samp_store_dur	samp_store_loc	samp_store_temp	secondary_treatment	sewage_type	sludge_retent_time	sodium	soluble_inorg_mat	soluble_org_mat	suspend_solids	temp	tertiary_treatment	tot_nitro	tot_phosphate	wastewater_type" << endl;
            }
        }else if (package == "Water") {
            out << "#MIMARKS.survey.water.4.0" << endl;
            if (requiredonly) {
                out << "#{sample name}	{description of sample}	{sample title}	{Date of sampling, in \"DD-Mmm-YYYY\", \"Mmm-YYYY\" or \"YYYY\" format (eg., 30-Oct-1990, Oct-1990 or 1990) or ISO 8601 standard \"YYYY-mm-dd\", \"YYYY-mm\" or \"YYYY-mm-ddThh:mm:ss\" (eg., 1990-10-30, 1990-10 or 1990-10-30T14:41:36)}	{descriptor of the broad ecological context of a sample. Examples include: desert, taiga, deciduous woodland, or coral reef. EnvO (v 2013-06-14) terms can be found via the link: www.environmentontology.org/Browse-EnvO}	{descriptor of the local environment. Examples include: harbor, cliff, or lake. EnvO (v 2013-06-14) terms can be found via the link: www.environmentontology.org/Browse-EnvO}	{material that was displaced by the sample, or material in which a sample was embedded, prior to the sampling event. Examples include: air, soil, or water. EnvO (v 2013-06-14) terms can be found via the link: www.environmentontology.org/Browse-EnvO}	{Geographical origin of the sample; use the appropriate name from this list http://www.insdc.org/documents/country-qualifier-vocabulary. Use a colon to separate the country or ocean from more detailed information about the location, eg \"Canada: Vancouver\" or \"Germany: halfway down Zugspitze, Alps\"}	{The geographical coordinates of the location where the sample was collected. Specify as degrees latitude and longitude in format \"d[d.dddd] N|S d[dd.dddd] W|E\", eg, 38.98 N 77.11 W}	{Depth is defined as the vertical distance below surface, e.g. for sediment or soil samples depth is measured from sediment or soil surface, respectivly. Depth can be reported as an interval for subsurface samples.}" << endl;
                out << "#{text}	{text}	{text}	{{timestamp}}	{{term}}	{{term}}	{{term}}	{{term}:{term}:{text}}	{{float} {float}}	{{float} m}" << endl;
                out << "*sample_name	*description	*sample_title	*collection_date	*env_biome	*env_feature	*env_material	*geo_loc_name	*lat_lon	*depth" << endl;
            }else {
                out << "#{sample name}	{description of sample}	{sample title}	{Date of sampling, in \"DD-Mmm-YYYY\", \"Mmm-YYYY\" or \"YYYY\" format (eg., 30-Oct-1990, Oct-1990 or 1990) or ISO 8601 standard \"YYYY-mm-dd\", \"YYYY-mm\" or \"YYYY-mm-ddThh:mm:ss\" (eg., 1990-10-30, 1990-10 or 1990-10-30T14:41:36)}	{descriptor of the broad ecological context of a sample. Examples include: desert, taiga, deciduous woodland, or coral reef. EnvO (v 2013-06-14) terms can be found via the link: www.environmentontology.org/Browse-EnvO}	{descriptor of the local environment. Examples include: harbor, cliff, or lake. EnvO (v 2013-06-14) terms can be found via the link: www.environmentontology.org/Browse-EnvO}	{material that was displaced by the sample, or material in which a sample was embedded, prior to the sampling event. Examples include: air, soil, or water. EnvO (v 2013-06-14) terms can be found via the link: www.environmentontology.org/Browse-EnvO}	{Geographical origin of the sample; use the appropriate name from this list http://www.insdc.org/documents/country-qualifier-vocabulary. Use a colon to separate the country or ocean from more detailed information about the location, eg \"Canada: Vancouver\" or \"Germany: halfway down Zugspitze, Alps\"}	{The geographical coordinates of the location where the sample was collected. Specify as degrees latitude and longitude in format \"d[d.dddd] N|S d[dd.dddd] W|E\", eg, 38.98 N 77.11 W}	{Depth is defined as the vertical distance below surface, e.g. for sediment or soil samples depth is measured from sediment or soil surface, respectivly. Depth can be reported as an interval for subsurface samples.}	{alkalinity, the ability of a solution to neutralize acids to the equivalence point of carbonate or bicarbonate}	{concentration of alkyl diethers}	{The altitude of the sample is the vertical distance between Earth's surface above Sea Level and the sampled position in the air.}	{measurement of aminopeptidase activity}	{concentration of ammonium}	{measurement of atmospheric data; can include multiple data}	{bacterial production in the water column measured by isotope uptake}	{measurement of bacterial respiration in the water column}	{measurement of bacterial carbon production}	{amount of biomass; should include the name for the part of biomass measured, e.g. microbial, total. can include multiple measurements}	{concentration of bishomohopanol}	{concentration of bromide}	{concentration of calcium}	{ratio of amount or concentrations of carbon to nitrogen}	{list of chemical compounds administered to the host or site where sampling occurred, and when (e.g. antibiotics, N fertilizer, air filter); can include multiple compounds. For Chemical Entities of Biological Interest ontology (CHEBI) (v1.72), please see http://bioportal.bioontology.org/visualize/44603}	{concentration of chloride}	{concentration of chlorophyll}	{electrical conductivity of water}	{density of sample}	{concentration of diether lipids; can include multiple types of diether lipids}	{concentration of dissolved carbon dioxide}	{concentration of dissolved hydrogen}	{dissolved inorganic carbon concentration}	{concentration of dissolved inorganic nitrogen}	{concentration of dissolved inorganic phosphorus}	{concentration of dissolved organic carbon}	{dissolved organic nitrogen concentration measured as; total dissolved nitrogen - NH4 - NO3 - NO2}	{concentration of dissolved oxygen}	{visible waveband radiance and irradiance measurements in the water column}	{The elevation of the sampling site as measured by the vertical distance from mean sea level.}	{raw or converted fluorescence of water}	{measurement of glucosidase activity}	{measurement of light intensity}	{concentration of magnesium}	{measurement of mean friction velocity}	{measurement of mean peak friction velocity}	{any other measurement performed or parameter collected, that is not listed here}	{concentration of n-alkanes; can include multiple n-alkanes}	{concentration of nitrate}	{concentration of nitrite}	{concentration of nitrogen (total)}	{concentration of organic carbon}	{concentration of organic matter}	{concentration of organic nitrogen}	{total count of any organism per gram or volume of sample,should include name of organism followed by count; can include multiple organism counts}	{oxygenation status of sample}	{concentration of particulate organic carbon}	{concentration of particulate organic nitrogen}	{type of perturbation, e.g. chemical administration, physical disturbance, etc., coupled with time that perturbation occurred; can include multiple perturbation types}	{concentration of petroleum hydrocarbon}	{pH measurement}	{concentration of phaeopigments; can include multiple phaeopigments}	{concentration of phosphate}	{concentration of phospholipid fatty acids; can include multiple values}	{measurement of photon flux}	{concentration of potassium}	{pressure to which the sample is subject, in atmospheres}	{measurement of primary production}	{redox potential, measured relative to a hydrogen cell, indicating oxidation or reduction potential}	{salinity measurement}	{none}	{none}	{none}	{concentration of silicate}	{sodium concentration}	{concentration of soluble reactive phosphorus}	{concentration of sulfate}	{concentration of sulfide}	{concentration of suspended particulate matter}	{temperature of the sample at time of sampling}	{stage of tide}	{measurement of total depth of water column}	{total dissolved nitrogen concentration, reported as nitrogen, measured by: total dissolved nitrogen = NH4 + NO3NO2 + dissolved organic nitrogen}	{total inorganic nitrogen content}	{total nitrogen content of the sample}	{total particulate carbon content}	{total phosphorus concentration, calculated by: total phosphorus = total dissolved phosphorus + particulate phosphorus. Can also be measured without filtering, reported as phosphorus}	{measurement of magnitude and direction of flow within a fluid}" << endl;
                out << "#{text}	{text}	{text}	{{timestamp}}	{{term}}	{{term}}	{{term}}	{{term}:{term}:{text}}	{{float} {float}}	{{float} m}	{{float} {unit}}	{{float} {unit}}	{{float} m}	{{float} {unit}}	{{float} {unit}}	{{text};{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{text};{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{term}; {timestamp}}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{text};{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{text};{float} {unit}}	{{text};{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{text};{float} {unit}}	{['', 'aerobic', 'anaerobic']}	{{float} {unit}}	{{float} {unit}}	{{text};{interval}}	{{float} {unit}}	{{float} {unit}}	{{text};{float} {unit}}	{{float} {unit}}	{{text};{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{none}}	{{none}}	{{none}}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{['', 'low', 'high']}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}	{{float} {unit}}" << endl;
                out << "*sample_name	*description	*sample_title	*collection_date	*env_biome	*env_feature	*env_material	*geo_loc_name	*lat_lon	*depth	alkalinity	alkyl_diethers	altitude	aminopept_act	ammonium	atmospheric_data	bac_prod	bac_resp	bacteria_carb_prod	biomass	bishomohopanol	bromide	calcium	carb_nitro_ratio	chem_administration	chloride	chlorophyll	conduc	density	diether_lipids	diss_carb_dioxide	diss_hydrogen	diss_inorg_carb	diss_inorg_nitro	diss_inorg_phosp	diss_org_carb	diss_org_nitro	diss_oxygen	down_par	elev	fluor	glucosidase_act	light_intensity	magnesium	mean_frict_vel	mean_peak_frict_vel	misc_param	n_alkanes	nitrate	nitrite	nitro	org_carb	org_matter	org_nitro	organism_count	oxy_stat_samp	part_org_carb	part_org_nitro	perturbation	petroleum_hydrocarb	ph	phaeopigments	phosphate	phosplipid_fatt_acid	photon_flux	potassium	pressure	primary_prod	redox_potential	salinity	samp_store_dur	samp_store_loc	samp_store_temp	silicate	sodium	soluble_react_phosp	sulfate	sulfide	suspend_part_matter	temp	tidal_stage	tot_depth_water_col	tot_diss_nitro	tot_inorg_nitro	tot_nitro	tot_part_carb	tot_phosp	water_current" << endl;
            }
        }

        for (set<string>::iterator it = Groups.begin(); it != Groups.end(); it++) {  out << *it << '\t' << endl; }
        
        out.close();
        
        //output files created by command
		m->mothurOutEndLine();
		m->mothurOut("Output File Names: "); m->mothurOutEndLine();
		for (int i = 0; i < outputNames.size(); i++) {	m->mothurOut(outputNames[i]); m->mothurOutEndLine();	}
		m->mothurOutEndLine();
        return 0;
		
    }
	catch(exception& e) {
		m->errorOut(e, "GetMIMarksPackageCommand", "execute");
		exit(1);
	}
}
//***************************************************************************************************************

// going to have to rework this to allow for other options --
/*
 file option 1
 
 sfffile1   oligosfile1
 sfffile2   oligosfile2
 ...
 
 file option 2
 
 fastqfile1 oligosfile1
 fastqfile2 oligosfile2
 ...
 
 file option 3
 
 ffastqfile1 rfastqfile1
 ffastqfile2 rfastqfile2
 ...
 
 file option 4
 
 group fastqfile  fastqfile
 group fastqfile  fastqfile
 group fastqfile  fastqfile
 ...
 
 file option 5
 
 My.forward.fastq My.reverse.fastq none My.rindex.fastq //none is an option is no forward or reverse index file
 ...
 */

int GetMIMarksPackageCommand::readFile(){
	try {
        
        inputfile = file;
        int format = 2;
        
        ifstream in;
        m->openInputFile(file, in);
        
        while(!in.eof()) {
            
            Oligos oligos;
            
            if (m->control_pressed) { return 0; }
            
            string line = m->getline(in);  m->gobble(in);
            vector<string> pieces = m->splitWhiteSpace(line);
            
            string group = "";
            string thisFileName1, thisFileName2; thisFileName1 = ""; thisFileName2 = "";
            if (pieces.size() == 2) {
                thisFileName1 = pieces[0];
                thisFileName2 = pieces[1];
            }else if (pieces.size() == 3) {
                thisFileName1 = pieces[1];
                thisFileName2 = pieces[2];
                group = pieces[0];
            }else if (pieces.size() == 4) {
                if (!setOligosParameter) { m->mothurOut("[ERROR]: You must have an oligosfile with the index file option. Aborting. \n"); m->control_pressed = true; }
                thisFileName1 = pieces[0];
                thisFileName2 = pieces[1];
            }else {
                m->mothurOut("[ERROR]: file lines can be 2, 3 or 4 columns. The 2 column files are sff file then oligos or fastqfile then oligos or ffastq and rfastq. You may have multiple lines in the file.  The 3 column files are for paired read libraries. The format is groupName, forwardFastqFile reverseFastqFile. Four column files are for inputting file pairs with index files. Example: My.forward.fastq My.reverse.fastq NONE My.rindex.fastq. The keyword NONE can be used when there is not a index file for either the forward or reverse file.\n"); m->control_pressed = true;
            }
            
            if (m->debug) { m->mothurOut("[DEBUG]: group = " + group + ", thisFileName1 = " + thisFileName1 + ", thisFileName2 = " + thisFileName2  + ".\n"); }
            
            if (inputDir != "") {
                string path = m->hasPath(thisFileName2);
                if (path == "") {  thisFileName2 = inputDir + thisFileName2;  }
                
                path = m->hasPath(thisFileName1);
                if (path == "") {  thisFileName1 = inputDir + thisFileName1;  }
            }
            
            //check to make sure both are able to be opened
            ifstream in2;
            int openForward = m->openInputFile(thisFileName1, in2, "noerror");
            
            //if you can't open it, try default location
            if (openForward == 1) {
                if (m->getDefaultPath() != "") { //default path is set
                    string tryPath = m->getDefaultPath() + m->getSimpleName(thisFileName1);
                    m->mothurOut("Unable to open " + thisFileName1 + ". Trying default " + tryPath); m->mothurOutEndLine();
                    ifstream in3;
                    openForward = m->openInputFile(tryPath, in3, "noerror");
                    in3.close();
                    thisFileName1 = tryPath;
                }
            }
            
            //if you can't open it, try output location
            if (openForward == 1) {
                if (m->getOutputDir() != "") { //default path is set
                    string tryPath = m->getOutputDir() + m->getSimpleName(thisFileName1);
                    m->mothurOut("Unable to open " + thisFileName1 + ". Trying output directory " + tryPath); m->mothurOutEndLine();
                    ifstream in4;
                    openForward = m->openInputFile(tryPath, in4, "noerror");
                    thisFileName1 = tryPath;
                    in4.close();
                }
            }
            
            if (openForward == 1) { //can't find it
                m->mothurOut("[WARNING]: can't find " + thisFileName1 + ", ignoring.\n");
            }else{  in2.close();  }
            
            int openReverse = 1;
            
            ifstream in3;
            openReverse = m->openInputFile(thisFileName2, in3, "noerror");
            
            //if you can't open it, try default location
            if (openReverse == 1) {
                if (m->getDefaultPath() != "") { //default path is set
                    string tryPath = m->getDefaultPath() + m->getSimpleName(thisFileName2);
                    m->mothurOut("Unable to open " + thisFileName2 + ". Trying default " + tryPath); m->mothurOutEndLine();
                    ifstream in3;
                    openReverse = m->openInputFile(tryPath, in3, "noerror");
                    in3.close();
                    thisFileName2 = tryPath;
                }
            }
            
            //if you can't open it, try output location
            if (openReverse == 1) {
                if (m->getOutputDir() != "") { //default path is set
                    string tryPath = m->getOutputDir() + m->getSimpleName(thisFileName2);
                    m->mothurOut("Unable to open " + thisFileName2 + ". Trying output directory " + tryPath); m->mothurOutEndLine();
                    ifstream in4;
                    openReverse = m->openInputFile(tryPath, in4, "noerror");
                    thisFileName2 = tryPath;
                    in4.close();
                }
            }
            
            if (openReverse == 1) { //can't find it
                m->mothurOut("[WARNING]: can't find " + thisFileName2 + ", ignoring pair.\n");
            }else{  in3.close();  }
            
            
            if ((pieces.size() == 2) && (openForward != 1) && (openReverse != 1)) { //good pair and sff or fastq and oligos
                    oligosfile = thisFileName2;
                    if (m->debug) { m->mothurOut("[DEBUG]: about to read oligos\n"); }
                    oligos.read(oligosfile);
                    createGroupNames(oligos); // adding in groupNames from this file
                format = 2;
            }else if((pieces.size() == 3) && (openForward != 1) && (openReverse != 1)) { //good pair and paired read
                Groups.insert(group);
                format = 3;
            }
        }
        in.close();
        
        inputfile = file;
        
        return 0;
    }
	catch(exception& e) {
		m->errorOut(e, "GetMIMarksPackageCommand", "readFile");
		exit(1);
	}
}
//**********************************************************************************************************************

set<string> GetMIMarksPackageCommand::createGroupNames(Oligos& oligos) {
    try {
        bool pairedOligos = false;
        
        if (oligos.hasPairedPrimers() || oligos.hasPairedBarcodes()) {      pairedOligos = true;        }
        
        vector<string> groupNames = oligos.getGroupNames();
        if (groupNames.size() == 0) { return Groups;  }
        
        if (pairedOligos) {
            //overwrite global oligos - assume fastq data like make.contigs
            Oligos oligos;
            if ((fileOption == 3) || (fileOption == 5)) { oligos.read(oligosfile, false);  } //like make.contigs
            else {  oligos.read(oligosfile);  }
            
            map<int, oligosPair> barcodes = oligos.getPairedBarcodes();
            map<int, oligosPair> primers = oligos.getPairedPrimers();
            for(map<int, oligosPair>::iterator itBar = barcodes.begin();itBar != barcodes.end();itBar++){
                for(map<int, oligosPair>::iterator itPrimer = primers.begin();itPrimer != primers.end(); itPrimer++){
                    
                    string primerName = oligos.getPrimerName(itPrimer->first);
                    string barcodeName = oligos.getBarcodeName(itBar->first);
                    
                    if ((primerName == "ignore") || (barcodeName == "ignore")) { } //do nothing
                    else if ((primerName == "") && (barcodeName == "")) { } //do nothing
                    else {
                        string comboGroupName = "";
                        string comboName = "";
                        
                        if(primerName == ""){
                            comboGroupName = barcodeName;
                        }else{
                            if(barcodeName == ""){
                                comboGroupName = primerName;
                            }
                            else{
                                comboGroupName = barcodeName + "." + primerName;
                            }
                        }
                        
                        if(((itPrimer->second).forward+(itPrimer->second).reverse) == ""){
                            if ((itBar->second).forward != "NONE") { comboName += (itBar->second).forward; }
                            if ((itBar->second).reverse != "NONE") {
                                if (comboName == "") {  comboName += (itBar->second).reverse; }
                                else {  comboName += ("."+(itBar->second).reverse);  }
                            }
                        }else{
                            if(((itBar->second).forward+(itBar->second).reverse) == ""){
                                if ((itPrimer->second).forward != "NONE") { comboName += (itPrimer->second).forward; }
                                if ((itPrimer->second).reverse != "NONE") {
                                    if (comboName == "") {  comboName += (itPrimer->second).reverse; }
                                    else {  comboName += ("."+(itPrimer->second).reverse);  }
                                }
                            }
                            else{
                                if ((itBar->second).forward != "NONE") { comboName += (itBar->second).forward; }
                                if ((itBar->second).reverse != "NONE") {
                                    if (comboName == "") {  comboName += (itBar->second).reverse; }
                                    else {  comboName += ("."+(itBar->second).reverse);  }
                                }
                                if ((itPrimer->second).forward != "NONE") {
                                    if (comboName == "") {  comboName += (itPrimer->second).forward; }
                                    else {  comboName += ("."+(itPrimer->second).forward);  }
                                }
                                if ((itPrimer->second).reverse != "NONE") {
                                    if (comboName == "") {  comboName += (itPrimer->second).reverse; }
                                    else {  comboName += ("."+(itPrimer->second).reverse);  }
                                }
                            }
                        }
                        
                        if (comboName != "") {  comboGroupName +=  "_" + comboName;  }
                        Groups.insert(comboGroupName);
                        
                        map<string, string>::iterator itGroup2Barcode = Group2Barcode.find(comboGroupName);
                        if (itGroup2Barcode == Group2Barcode.end()) {
                            string temp = (itBar->second).forward+"."+(itBar->second).reverse;
                            Group2Barcode[comboGroupName] = temp;
                        }else {
                            string temp = (itBar->second).forward+"."+(itBar->second).reverse;
                            if ((temp != ".") && (temp != itGroup2Barcode->second)) {
                                m->mothurOut("[ERROR]: groupName = " + comboGroupName + "\t" + temp + "\t" + itGroup2Barcode->second + " group and barcodes/primers not unique. Should never get here.\n");
                            }
                        }
                        
                        itGroup2Barcode = Group2Primer.find(comboGroupName);
                        if (itGroup2Barcode == Group2Primer.end()) {
                            string temp = ((itPrimer->second).forward+"."+(itPrimer->second).reverse);
                            Group2Primer[comboGroupName] = temp;
                        }else {
                            string temp = ((itPrimer->second).forward+"."+(itPrimer->second).reverse);
                            if ((temp != ".") && (temp != itGroup2Barcode->second)) {
                                m->mothurOut("[ERROR]: groupName = " + comboGroupName + "\t" + temp + "\t" + itGroup2Barcode->second + " group and barcodes/primers not unique. Should never get here.\n");
                            }
                        }

                    }
                }
            }
        }else {
            map<string, int> barcodes = oligos.getBarcodes() ;
            map<string, int> primers = oligos.getPrimers();
            for(map<string, int>::iterator itBar = barcodes.begin();itBar != barcodes.end();itBar++){
                for(map<string, int>::iterator itPrimer = primers.begin();itPrimer != primers.end(); itPrimer++){
                    
                    string primerName = oligos.getPrimerName(itPrimer->second);
                    string barcodeName = oligos.getBarcodeName(itBar->second);
                    
                    if ((primerName == "ignore") || (barcodeName == "ignore")) { } //do nothing
                    else if ((primerName == "") && (barcodeName == "")) { } //do nothing
                    else {
                        string comboGroupName = "";
                        string comboName = "";
                        
                        if(primerName == ""){
                            comboGroupName = barcodeName;
                        }else{
                            if(barcodeName == ""){
                                comboGroupName = primerName;
                            }
                            else{
                                comboGroupName = barcodeName + "." + primerName;
                            }
                        }
                        
                        if(itPrimer->first == ""){
                            comboName = itBar->first;
                        }else{
                            if(itBar->first == ""){
                                comboName = itPrimer->first;
                            }
                            else{
                                comboName = itBar->first + "." + itPrimer->first;
                            }
                        }
                        
                        if (comboName != "") {  comboGroupName +=  "_" + comboName;  }
                        Groups.insert(comboGroupName);
                        
                        map<string, string >::iterator itGroup2Barcode = Group2Barcode.find(comboGroupName);
                        if (itGroup2Barcode == Group2Barcode.end()) {
                            string temp = (itBar->first);
                            Group2Barcode[comboGroupName] = temp;
                        }else {
                            string temp = (itBar->first);
                            if ((temp != ".") && (temp != itGroup2Barcode->second)) {
                                m->mothurOut("[ERROR]: groupName = " + comboGroupName + "\t" + temp + "\t" + itGroup2Barcode->second + " group and barcodes/primers not unique. Should never get here.\n");
                            }
                        }
                        
                        itGroup2Barcode = Group2Primer.find(comboGroupName);
                        if (itGroup2Barcode == Group2Primer.end()) {
                            string temp = (itPrimer->first);
                            Group2Primer[comboGroupName] = temp;
                        }else {
                            string temp = (itPrimer->first);
                            if ((temp != ".") && (temp != itGroup2Barcode->second)) {
                               m->mothurOut("[ERROR]: groupName = " + comboGroupName + "\t" + temp + "\t" + itGroup2Barcode->second + " group and barcodes/primers not unique. Should never get here.\n");
                            }
                        }

                    }
                }
            }
        }
        
        if (Groups.size() == 0) {
            m->mothurOut("[ERROR]: your oligos file does not contain any group names."); m->mothurOutEndLine(); m->control_pressed = true;
        }
        
        return Groups;
    }
    catch(exception& e) {
        m->errorOut(e, "GetMIMarksPackageCommand", "createGroupNames");
        exit(1);
    }
}
//**********************************************************************************************************************
/*
 file option 1
 
 sfffile1   oligosfile1
 sfffile2   oligosfile2
 ...
 
 file option 2
 
 fastqfile1 oligosfile1
 fastqfile2 oligosfile2
 ...
 
 file option 3
 
 ffastqfile1 rfastqfile1
 ffastqfile2 rfastqfile2
 ...
 
 file option 4
 
 group fastqfile  fastqfile
 group fastqfile  fastqfile
 group fastqfile  fastqfile
 ...
 
 file option 5
 
 My.forward.fastq My.reverse.fastq none My.rindex.fastq //none is an option is no forward or reverse index file
 ...
 */

int GetMIMarksPackageCommand::findFileOption(){
    try {
        ifstream in;
        m->openInputFile(file, in);
        
        fileOption = 0;
        
        while(!in.eof()) {
            
            if (m->control_pressed) { return 0; }
            
            string line = m->getline(in);  m->gobble(in);
            vector<string> pieces = m->splitWhiteSpace(line);
            
            if (pieces.size() == 2) { //good pair and sff or fastq and oligos
                if (!setOligosParameter) {
                    fileOption = 12; //1 or 2
                }else {  fileOption = 3; }
            }else if(pieces.size() == 3) { //good pair and paired read
                fileOption = 4;
            }else if (pieces.size() == 4) {
                fileOption = 5;
            }
            break;
        }
        in.close();
        
        return fileOption;
    }
    catch(exception& e) {
        m->errorOut(e, "GetMIMarksPackageCommand", "findFileOption");
        exit(1);
    }
}

//**********************************************************************************************************************




imgProjects = new Image();
imgProjects.src = "images/projects.gif";
imgProjects_g = new Image();
imgProjects_g.src = "images/projects_g.gif";
imgCourses = new Image();
imgCourses.src = "images/courses.gif";
imgCourses_g = new Image();
imgCourses_g.src = "images/courses_g.gif";
imgResume = new Image();
imgResume.src = "images/resume.gif";
imgResume_g = new Image();
imgResume_g.src = "images/resume_g.gif";
imgPhotos = new Image();
imgPhotos.src = "images/photos.gif";
imgPhotos_g = new Image();
imgPhotos_g.src = "images/photos_g.gif";
imgDoneThat = new Image();
imgDoneThat.src = "images/donethat.gif";
imgDoneThat_g = new Image();
imgDoneThat_g.src = "images/donethat_g.gif";

function reDisplay(selection, subSelection) {

	toc.document.open();
	toc.document.write("<html>\n<head>\n<title>ToC</title>\n</head>\n<body bgcolor=\"#D6CEBD\">\n");

	toc.document.writeln('<center>');

	toc.document.writeln('<A HREF="#" onClick="parent.reDisplay(\'courses\')" ><img name="imgC" src="images/courses.gif" onMouseOver="document.imgC.src=parent.imgCourses_g.src" onMouseOut="document.imgC.src=parent.imgCourses.src"  align=center border=0></A></br>');
	if (selection == "courses") {
	    toc.document.writeln('</br>');
		toc.document.writeln('<A HREF="csci6166/csci6166.html" target="content" style="text-decoration:none"><img src="images/bookshut_brown.gif" border=0>&nbsp; csci6166</A></br>');
		toc.document.writeln('<A HREF="csci6162/csci6162.html" target="content" style="text-decoration:none"><img src="images/bookshut_brown.gif" border=0>&nbsp; csci6162</A></br>');
		toc.document.writeln('<A HREF="csci6160/csci6160.html" target="content" style="text-decoration:none"><img src="images/bookshut_brown.gif" border=0>&nbsp; csci6160</A></br>');
		toc.document.writeln('<A HREF="csci6148/csci6148.html" target="content" style="text-decoration:none"><img src="images/bookshut_brown.gif" border=0>&nbsp; csci6148</A></br>');
		toc.document.writeln('<A HREF="csci6144/csci6144.html" target="content" style="text-decoration:none"><img src="images/bookshut_brown.gif" border=0>&nbsp csci6144</A></br>');
		toc.document.writeln('<A HREF="csci6114/csci6114.html" target="content" style="text-decoration:none"><img src="images/bookshut_brown.gif" border=0>&nbsp csci6114</A></br>');
		toc.document.writeln('<A HREF="csci5141/csci5141.html" target="content" style="text-decoration:none"><img src="images/bookshut_brown.gif" border=0>&nbsp csci5141</A></br>');
		toc.document.writeln('<A HREF="csci5130/csci5130.html" target="content" style="text-decoration:none"><img src="images/bookshut_brown.gif" border=0>&nbsp csci5130</A></br>');
		toc.document.writeln('<A HREF="csci5112/csci5112.html" target="content" style="text-decoration:none"><img src="images/bookshut_brown.gif" border=0>&nbsp csci5112</A></br>');
		toc.document.writeln('<A HREF="csci5102/csci5102.html" target="content" style="text-decoration:none"><img src="images/bookshut_brown.gif" border=0>&nbsp csci5102</A></br>');
		toc.document.writeln("</br>");

		parent.content.location.href="courses_summary.html"
	}
	toc.document.writeln("</br>");






	toc.document.writeln('<A HREF="#" onClick="parent.reDisplay(\'projects\')"><img name="imgP" src="images/projects.gif" onMouseOver="document.imgP.src=parent.imgProjects_g.src" onMouseOut="document.imgP.src=parent.imgProjects.src"  align=center border=0></A></br>');
	if (selection == "projects") {
		parent.content.location.href="projects_summary.html"
	}
	toc.document.writeln("</br>");

	toc.document.writeln('<A HREF="#" onClick="parent.reDisplay(\'resume\')"><img name="imgR" src="images/resume.gif" onMouseOver="document.imgR.src=parent.imgResume_g.src" onMouseOut="document.imgR.src=parent.imgResume.src"  align=center border=0></A></br>');
	if (selection == "resume") {
	    toc.document.writeln('</br>');
	    toc.document.writeln('</center>');
		toc.document.writeln('&nbsp;&nbsp;<A HREF="resume/resume.html" target="content" style="text-decoration:none"><img src="images/resume_bullet.gif" border=0>&nbsp; HTML</A></br>');
		toc.document.writeln('&nbsp;&nbsp;<A HREF="resume/resume.doc" target="content" style="text-decoration:none"><img src="images/resume_bullet.gif" border=0>&nbsp; Word</A></br>');
		toc.document.writeln('&nbsp;&nbsp;<A HREF="resume/resume.txt" target="content" style="text-decoration:none"><img src="images/resume_bullet.gif" border=0>&nbsp; Text</A></br>');
	    toc.document.writeln('<center>');
		parent.content.location.href="resume/resume_summary.html"
	}
	toc.document.writeln("</br>");

	toc.document.writeln('<A HREF="#" onClick="parent.reDisplay(\'photos\')"><img name="imgPh" src="images/photos.gif" onMouseOver="document.imgPh.src=parent.imgPhotos_g.src" onMouseOut="document.imgPh.src=parent.imgPhotos.src"  align=center border=0></A></br>');
	if (selection == "photos") {
		parent.content.location.href="photos/photos.html"
	}
	toc.document.writeln("</br>");

	toc.document.writeln('<A HREF="#" onClick="parent.reDisplay(\'donethat\')"><img name="imgDt" src="images/donethat.gif" onMouseOver="document.imgDt.src=parent.imgDoneThat_g.src" onMouseOut="document.imgDt.src=parent.imgDoneThat.src"  align=center border=0></A></br>');
	if (selection == "donethat") {
		parent.content.location.href="donethat/donethat.html"
	}

	toc.document.writeln('</center>');

	toc.document.writeln("</body>");
	toc.document.close();
	
}

var imgProjects = new Image();
imgProjects.src = "images/projects.gif";
var imgProjects_g = new Image();
imgProjects_g.src = "images/projects_g.gif";
var imgCourses = new Image();
imgCourses.src = "images/courses.gif";
var imgCourses_g = new Image();
imgCourses_g.src = "images/courses_g.gif";
var imgResume = new Image();
imgResume.src = "images/resume.gif";
var imgResume_g = new Image();
imgResume_g.src = "images/resume_g.gif";
var imgPhotos = new Image();
imgPhotos.src = "images/photos.gif";
var imgPhotos_g = new Image();
imgPhotos_g.src = "images/photos_g.gif";
var imgDoneThat = new Image();
imgDoneThat.src = "images/donethat.gif";
var imgDoneThat_g = new Image();
imgDoneThat_g.src = "images/donethat_g.gif";

function loadContent(url) {
	var frame = document.getElementById('content');
	if (frame) frame.src = url;
}

function reDisplay(selection, subSelection) {

	var html = '<center>\n';

	html += '<a href="#" onclick="reDisplay(\'courses\'); return false;"><img src="images/courses.gif" onmouseover="this.src=imgCourses_g.src" onmouseout="this.src=imgCourses.src" align="center" border="0"></a><br>\n';
	if (selection == "courses") {
		html += '<br>\n';
		html += '<a href="csci6166/csci6166.html" onclick="loadContent(\'csci6166/csci6166.html\'); return false;" style="text-decoration:none"><img src="images/bookshut_brown.gif" border="0">&nbsp; csci6166</a><br>\n';
		html += '<a href="csci6162/csci6162.html" onclick="loadContent(\'csci6162/csci6162.html\'); return false;" style="text-decoration:none"><img src="images/bookshut_brown.gif" border="0">&nbsp; csci6162</a><br>\n';
		html += '<a href="csci6160/csci6160.html" onclick="loadContent(\'csci6160/csci6160.html\'); return false;" style="text-decoration:none"><img src="images/bookshut_brown.gif" border="0">&nbsp; csci6160</a><br>\n';
		html += '<a href="csci6148/csci6148.html" onclick="loadContent(\'csci6148/csci6148.html\'); return false;" style="text-decoration:none"><img src="images/bookshut_brown.gif" border="0">&nbsp; csci6148</a><br>\n';
		html += '<a href="csci6144/csci6144.html" onclick="loadContent(\'csci6144/csci6144.html\'); return false;" style="text-decoration:none"><img src="images/bookshut_brown.gif" border="0">&nbsp; csci6144</a><br>\n';
		html += '<a href="csci6114/csci6114.html" onclick="loadContent(\'csci6114/csci6114.html\'); return false;" style="text-decoration:none"><img src="images/bookshut_brown.gif" border="0">&nbsp; csci6114</a><br>\n';
		html += '<a href="csci5141/csci5141.html" onclick="loadContent(\'csci5141/csci5141.html\'); return false;" style="text-decoration:none"><img src="images/bookshut_brown.gif" border="0">&nbsp; csci5141</a><br>\n';
		html += '<a href="csci5130/csci5130.html" onclick="loadContent(\'csci5130/csci5130.html\'); return false;" style="text-decoration:none"><img src="images/bookshut_brown.gif" border="0">&nbsp; csci5130</a><br>\n';
		html += '<a href="csci5112/csci5112.html" onclick="loadContent(\'csci5112/csci5112.html\'); return false;" style="text-decoration:none"><img src="images/bookshut_brown.gif" border="0">&nbsp; csci5112</a><br>\n';
		html += '<a href="csci5102/csci5102.html" onclick="loadContent(\'csci5102/csci5102.html\'); return false;" style="text-decoration:none"><img src="images/bookshut_brown.gif" border="0">&nbsp; csci5102</a><br>\n';
		html += '<br>\n';
		loadContent('courses_summary.html');
	}
	html += '<br>\n';

	html += '<a href="#" onclick="reDisplay(\'projects\'); return false;"><img src="images/projects.gif" onmouseover="this.src=imgProjects_g.src" onmouseout="this.src=imgProjects.src" align="center" border="0"></a><br>\n';
	if (selection == "projects") {
		loadContent('projects_summary.html');
	}
	html += '<br>\n';

	html += '<a href="#" onclick="reDisplay(\'resume\'); return false;"><img src="images/resume.gif" onmouseover="this.src=imgResume_g.src" onmouseout="this.src=imgResume.src" align="center" border="0"></a><br>\n';
	if (selection == "resume") {
		html += '<br></center>\n';
		html += '&nbsp;&nbsp;<a href="resume/resume.html" onclick="loadContent(\'resume/resume.html\'); return false;" style="text-decoration:none"><img src="images/resume_bullet.gif" border="0">&nbsp; HTML</a><br>\n';
		html += '&nbsp;&nbsp;<a href="resume/resume.doc" target="content" style="text-decoration:none"><img src="images/resume_bullet.gif" border="0">&nbsp; Word</a><br>\n';
		html += '<center>\n';
		loadContent('resume/resume_summary.html');
	}
	html += '<br>\n';

	html += '<a href="#" onclick="reDisplay(\'photos\'); return false;"><img src="images/photos.gif" onmouseover="this.src=imgPhotos_g.src" onmouseout="this.src=imgPhotos.src" align="center" border="0"></a><br>\n';
	if (selection == "photos") {
		loadContent('photos/photos.html');
	}
	html += '<br>\n';

	html += '<a href="#" onclick="reDisplay(\'donethat\'); return false;"><img src="images/donethat.gif" onmouseover="this.src=imgDoneThat_g.src" onmouseout="this.src=imgDoneThat.src" align="center" border="0"></a><br>\n';
	if (selection == "donethat") {
		loadContent('donethat/donethat.html');
	}

	html += '</center>\n';

	document.getElementById('toc').innerHTML = html;
}

function ComputeGPA()
{
	var gradeSum = 0;
	var scoreSum = 0;
	var i = 1;
	for(;i<=scoreTbl.rows.length-1;i++)
		if(scoreTbl.rows(i).childNodes(0).childNodes(0).checked)
		{
			scoreSum = scoreSum + parseFloat(scoreTbl.rows(i).childNodes(3).innerText);
			gradeSum = gradeSum + parseFloat(scoreTbl.rows(i).childNodes(5).innerText) * parseFloat(scoreTbl.rows(i).childNodes(3).innerText);
		}
    disp.innerText = gradeSum/scoreSum;
}

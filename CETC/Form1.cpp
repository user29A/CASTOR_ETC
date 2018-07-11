/*Copyright 2018 Joseph Edwin Postma

Contact email: joepostma@live.ca

This file is part of CETC.

CETC is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

CETC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

See http://www.gnu.org/licenses/ for a copy of the
GNU General Public License. */


#include "Form1.h"
using namespace System;
using namespace System::Windows::Forms;
using namespace CETC;

[STAThread]
int Main(array<String^>^ startargs)
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	
	/*CETC::Form1 form;
	Application::Run(%form);*/

	if (startargs->Length == 0)
		Application::Run(gcnew Form1());
	else
		Application::Run(gcnew Form1(startargs));

	return 0;
}

void Form1::InitializeVars(array<String^>^ startargs)
{
	STARTARGS = startargs;
}

void Form1::Form1_Load(System::Object^  sender, System::EventArgs^  e)
{
	FIRSTSLOAD = true;

	SourceGalaxyDrop->SelectedIndex = ::Convert::ToInt32(JPRegistry::GetReg("CETC", "SourceGalaxyDropIndex"));
	SourceStarDrop->SelectedIndex = ::Convert::ToInt32(JPRegistry::GetReg("CETC", "SourceStarDropIndex"));
	SourceBlackbodyTempTxt->Text = (String^)JPRegistry::GetReg("CETC", "SourceBlackbodyTempTxt");
	FilterUVRadBtn->Checked = ::Convert::ToBoolean(JPRegistry::GetReg("CETC", "FilterUVRadBtnChckd"));
	FilteruRadBtn->Checked = ::Convert::ToBoolean(JPRegistry::GetReg("CETC", "FilteruRadBtnChckd"));
	FiltergRadBtn->Checked = ::Convert::ToBoolean(JPRegistry::GetReg("CETC", "FiltergRadBtnChckd"));
	FilterUVDarkRadBtn->Checked = ::Convert::ToBoolean(JPRegistry::GetReg("CETC", "FilterUVDarkRadBtnChckd"));
	FilteruWideRadBtn->Checked = ::Convert::ToBoolean(JPRegistry::GetReg("CETC", "FilterUWideRadBtnChckd"));
	SourceStarRadBtn->Checked = ::Convert::ToBoolean(JPRegistry::GetReg("CETC", "SourceStarRadBtn"));
	SourceGalaxyRadBtn->Checked = ::Convert::ToBoolean(JPRegistry::GetReg("CETC", "SourceGalaxyRadBtn"));
	SourceBlackbodyRadBtn->Checked = ::Convert::ToBoolean(JPRegistry::GetReg("CETC", "SourceBlackbodyRadBtn"));
	SourceAGNRadBtn->Checked = ::Convert::ToBoolean(JPRegistry::GetReg("CETC", "SourceAGNRadBtn"));
	SourceAGNDrop->SelectedIndex = ::Convert::ToInt32(JPRegistry::GetReg("CETC", "SourceAGNDropIndex"));
	SourcePowerLawRadBtn->Checked = ::Convert::ToBoolean(JPRegistry::GetReg("CETC", "SourcePowerLawRadBtn"));
	SourcePowerLawDrop->SelectedIndex = ::Convert::ToInt32(JPRegistry::GetReg("CETC", "SourcePowerLawDropIndex"));
	DistanceRadBtn->Checked = ::Convert::ToBoolean(JPRegistry::GetReg("CETC", "DistanceRadBtn"));
	mvRadBtn->Checked = ::Convert::ToBoolean(JPRegistry::GetReg("CETC", "mvRadBtn"));
	ExtinctionColumnDensityTxt->Text = (String^)JPRegistry::GetReg("CETC", "ExtinctionColumnDensityTxt");
	ExtinctionAvTxt->Text = (String^)JPRegistry::GetReg("CETC", "ExtinctionAvTxt");
	ExtinctionRvTxt->Text = (String^)JPRegistry::GetReg("CETC", "ExtinctionRvTxt");
	ExtinctionColumnDensityRadBtn->Checked = ::Convert::ToBoolean(JPRegistry::GetReg("CETC", "ExtinctionColumnDensityRadBtn"));
	ExtinctionDistanceRadBtn->Checked = ::Convert::ToBoolean(JPRegistry::GetReg("CETC", "ExtinctionDistanceRadBtn"));
	ExtinctionAvRadBtn->Checked = ::Convert::ToBoolean(JPRegistry::GetReg("CETC", "ExtinctionAvRadBtn"));
	RadiusTxt->Text = (String^)JPRegistry::GetReg("CETC", "RadiusTxt");
	mvTxt->Text = (String^)JPRegistry::GetReg("CETC", "mvTxt");
	DistanceTxt->Text = (String^)JPRegistry::GetReg("CETC", "DistanceTxt");
	RedShiftTxt->Text = (String^)JPRegistry::GetReg("CETC", "RedShiftTxt");
	mTxt->Text = (String^)JPRegistry::GetReg("CETC", "mTxt");
	PowerLawNormTxt->Text = (String^)JPRegistry::GetReg("CETC", "PowerLawNormTxt");
	PowerLawAlphaTxt->Text = (String^)JPRegistry::GetReg("CETC", "PowerLawAlphaTxt");
	SNTargetUpD->Value = ::Convert::ToDecimal(JPRegistry::GetReg("CETC", "SNTargetUpD"));
	PlateScaleUpD->Value = ::Convert::ToDecimal(JPRegistry::GetReg("CETC", "PlateScaleUpD"));
	FWHMUpD->Value = ::Convert::ToDecimal(JPRegistry::GetReg("CETC", "FWHMUpD"));

	LAMBDA_NM = JPFits::FitsFile::ReadImageVectorOnly(LAMBDA_NM_FILENAME, nullptr);
	
	DETECTOR_QE = JPFits::FitsFile::ReadImageVectorOnly(QE_FILENAME, nullptr);
	
	BACKGROUND = JPFits::FitsFile::ReadImageVectorOnly(BACKGROUND_FILENAME, nullptr); 
	//BG is in log values; needs to be converted to SI, from (erg/cm^2/s/A/arcsec^2) to (J/m^2/s/m/9pix^2)
	//= 1x10^-7 (J/erg) / 1x10^-4 (m^2/cm^2) / 1x10^-10 (m/A) / 1/97.2 (9pix^2/arcsec^2) = 9.72x10^8
	for (int i = 0; i < LAMBDA_NM->Length; i++)
		BACKGROUND[i] = Math::Pow(10, BACKGROUND[i]) * 9.72e8;

	MIRROR_REFL = JPFits::FitsFile::ReadImageVectorOnly(MIRROR_REFL_FILENAME, nullptr);

	System::IO::StreamReader^ sr = gcnew System::IO::StreamReader(MIRROR_EFF_AREA_FILENAME);
	MIRROR_EFF_AREA = Convert::ToDouble(sr->ReadLine());
	sr->Close();

	//uvdark, uv, uwide, u, g,
	FILTER_FILENAMES[0] = FILTER_uvdark_FILENAME;
	FILTER_FILENAMES[1] = FILTER_uv_FILENAME;
	FILTER_FILENAMES[2] = FILTER_uwide_FILENAME;
	FILTER_FILENAMES[3] = FILTER_u_FILENAME;
	FILTER_FILENAMES[4] = FILTER_g_FILENAME;
	for (int i = 0; i < FILTER_FILENAMES->Length; i++)
	{
		array<double>^ filter = JPFits::FitsFile::ReadImageVectorOnly(FILTER_FILENAMES[i], nullptr);
		for (int j = 0; j < LAMBDA_NM->Length; j++)
			FILTERS[j, i] = filter[j];
	}

	FIRSTSLOAD = false;

	if (STARTARGS->Length > 0)//then this GUI been opened with a string argument for script execution
	{
		//TO DO:  Should supress computation until all switches are set, unlike the GUI interface where switch changes
		//trigger the computation and graph updates etc.
		//STARTARGS->Length > 0 is the bool that tells if script start




		//read the argument file name and set all the parameters, then execute
		String^ inputfile = STARTARGS[0];

		System::IO::StreamReader^ sr = gcnew System::IO::StreamReader(inputfile);
		String^ line = sr->ReadLine();

		if (line != "SESSION")
		{
			::MessageBox::Show("Input file error at SESSION.  Stopping!");
			return;
		}
		line = sr->ReadLine();
		try
		{
			SESSION = Convert::ToInt32(line);
		}
		catch (...)
		{
			::MessageBox::Show("Input file error at SESSION number: not an integer.  Stopping!");
			return;
		}
		//SESSION number now set

		line = sr->ReadLine();
		if (line != "SOURCE")
		{
			::MessageBox::Show("Input file error at SOURCE.  Stopping!");
			return;
		}
		line = sr->ReadLine();  //ST | BB | GT | AGN | PL
		if (line == "ST")
			SourceStarRadBtn->Checked = true;
		else if (line == "BB")
			SourceBlackbodyRadBtn->Checked = true;
		else if (line == "GT")
			SourceGalaxyRadBtn->Checked = true;
		else if (line = "AGN")
			SourceAGNRadBtn->Checked = true;
		else if (line == "PL")
			SourcePowerLawRadBtn->Checked = true;
		else
		{
			::MessageBox::Show("Input file error at SOURCE type: None of ST or BB or GT or AGN or PL.  Stopping!");
			return;
		}
		//source radio button now set




		sr->Close();

		//now compute
		ThroughPutETC();//this will need to be called when/if the switch changes are supressed for script execution...otherwise for now it is already done
		//now save outputs

		String^ outdir = CASTOR_DIRECTORY + "\\Output\\" + SESSION.ToString() + "\\";
		IO::Directory::CreateDirectory(outdir);
		
		System::Drawing::Bitmap^ bmp = gcnew System::Drawing::Bitmap(Chart_Source->Width, Chart_Source->Height);
		Chart_Source->DrawToBitmap(bmp, Chart_Source->DisplayRectangle);
		bmp->Save(outdir + "source_" + SESSION.ToString() + ".jpeg");

		bmp = gcnew System::Drawing::Bitmap(Chart_Filter->Width, Chart_Filter->Height);
		Chart_Filter->DrawToBitmap(bmp, Chart_Filter->DisplayRectangle);
		bmp->Save(outdir + "filter_" + SESSION.ToString() + ".jpeg");

		bmp = gcnew System::Drawing::Bitmap(Chart_Final->Width, Chart_Final->Height);
		Chart_Final->DrawToBitmap(bmp, Chart_Final->DisplayRectangle);
		bmp->Save(outdir + "final_" + SESSION.ToString() + ".jpeg");
	}
}

void Form1::EscBtn_Click(System::Object^  sender, System::EventArgs^  e)
{
	if (MessageBox::Show("Are you sure you want to exit?", "Exit", ::MessageBoxButtons::YesNo) == ::DialogResult::No)
		return;

	Application::Exit();
}

void Form1::ShowLocalFlux_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
{
	if (!COMPUTATION_EXISTS)
		return;

	if (ShowLocalFlux->Checked)
		Chart_Source->Titles[0]->Text = "Local Source Flux (Source * Extinction * Distance)";
	else
		Chart_Source->Titles[0]->Text = "Source Flux";

	Chart_Source->Series[0]->Points->Clear();
	if (ShowLocalFlux->Checked)
		for (int i = 0; i < LAMBDA_NM->Length; i++)
			Chart_Source->Series[0]->Points->AddXY(LAMBDA_NM[i], SOURCE_FLUX_LOCAL[i]);
	else
		for (int i = 0; i < LAMBDA_NM->Length; i++)
			Chart_Source->Series[0]->Points->AddXY(LAMBDA_NM[i], SOURCE_FLUX[i]);
	Chart_Source->ChartAreas[0]->AxisX->Minimum = PLOT_MIN;
	Chart_Source->ChartAreas[0]->AxisX->Maximum = PLOT_MAX;
}

void Form1::SourceStarRadBtn_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
{
	if (SourceStarRadBtn->Checked)
	{
		mvRadBtn->Enabled = true;
		SourceStarDrop->Enabled = true;
		DistanceRadBtn->Enabled = true;
		if (DistanceRadBtn->Checked)
			DistanceTxt->Enabled = true;
		else
			DistanceTxt->Enabled = false;
		mvRadBtn->Enabled = true;
		if (mvRadBtn->Checked)
			mvTxt->Enabled = true;
		else
			mvTxt->Enabled = false;
		SourceBlackbodyTempTxt->Enabled = false;
		RadiusTxt->Enabled = true;
		PowerLawAlphaTxt->Enabled = false;
		PowerLawNormTxt->Enabled = false;
		mTxt->Enabled = false;
		RedShiftTxt->Enabled = false;
		ISPOINTSOURCE = true;

		if (FIRSTSLOAD)
			return;

		ThroughPutETC();
	}
	else
	{
		SourceStarDrop->Enabled = false;
		mvRadBtn->Enabled = false;
	}

	JPRegistry::SetReg("CETC", "SourceStarRadBtn", SourceStarRadBtn->Checked);
}

void Form1::SourceStarDrop_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
{
	if (FIRSTSLOAD)
		return;

	JPRegistry::SetReg("CETC", "SourceStarDropIndex", SourceStarDrop->SelectedIndex);

	ThroughPutETC();
}

void Form1::SourceAGNRadBtn_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
{
	if (SourceAGNRadBtn->Checked)
	{
		SourceAGNDrop->Enabled = true;
		RedShiftTxt->Enabled = true;
		mTxt->Enabled = true;
		mvRadBtn->Enabled = false;
		mvTxt->Enabled = false;
		PowerLawAlphaTxt->Enabled = false;
		PowerLawNormTxt->Enabled = false;
		DistanceRadBtn->Enabled = false;
		DistanceTxt->Enabled = false;
		RadiusTxt->Enabled = false;
		ISPOINTSOURCE = false;

		if (FIRSTSLOAD)
			return;
		ThroughPutETC();
	}
	else
	{
		SourceAGNDrop->Enabled = false;
		RedShiftTxt->Enabled = false;
		mTxt->Enabled = false;
	}

	JPRegistry::SetReg("CETC", "SourceAGNRadBtn", SourceAGNRadBtn->Checked);
}

void Form1::SourceGalaxyRadBtn_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
{
	if (SourceGalaxyRadBtn->Checked)
	{
		SourceGalaxyDrop->Enabled = true;
		RedShiftTxt->Enabled = true;
		mTxt->Enabled = true;
		mvRadBtn->Enabled = false;
		mvTxt->Enabled = false;
		PowerLawAlphaTxt->Enabled = false;
		PowerLawNormTxt->Enabled = false;
		DistanceRadBtn->Enabled = false;
		DistanceTxt->Enabled = false;
		RadiusTxt->Enabled = false;
		ISPOINTSOURCE = false;

		if (FIRSTSLOAD)
			return;
		ThroughPutETC();
	}
	else
	{
		SourceGalaxyDrop->Enabled = false;
		RedShiftTxt->Enabled = false;
		mTxt->Enabled = false;
	}

	JPRegistry::SetReg("CETC", "SourceGalaxyRadBtn", SourceGalaxyRadBtn->Checked);
}

void Form1::SourceGalaxyDrop_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
{
	if (FIRSTSLOAD)
		return;

	JPRegistry::SetReg("CETC", "SourceGalaxyDropIndex", SourceGalaxyDrop->SelectedIndex);

	ThroughPutETC();
}

void Form1::SourceBlackbodyRadBtn_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
{
	if (SourceBlackbodyRadBtn->Checked)
	{
		SourceBlackbodyTempTxt->Enabled = true;
		DistanceRadBtn->Enabled = true;
		DistanceRadBtn->Checked = true;
		RadiusTxt->Enabled = true;
		DistanceTxt->Enabled = true;
		mvRadBtn->Enabled = false;
		mvTxt->Enabled = false;
		PowerLawAlphaTxt->Enabled = false;
		PowerLawNormTxt->Enabled = false;
		mTxt->Enabled = false;
		RedShiftTxt->Enabled = false;
		ISPOINTSOURCE = true;

		if (FIRSTSLOAD)
			return;

		ThroughPutETC();
	}
	else
		SourceBlackbodyTempTxt->Enabled = false;

	JPRegistry::SetReg("CETC", "SourceBlackbodyRadBtn", SourceBlackbodyRadBtn->Checked);
}

void Form1::DistanceRadBtn_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
{
	if (DistanceRadBtn->Checked)
	{
		DistanceTxt->Enabled = true;
		if (FIRSTSLOAD)
			return;
		ThroughPutETC();
	}
	else
		DistanceTxt->Enabled = false;

	JPRegistry::SetReg("CETC", "DistanceRadBtn", DistanceRadBtn->Checked);
}

void Form1::mvRadBtn_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
{
	if (mvRadBtn->Checked)
	{
		mvTxt->Enabled = true;
		if (FIRSTSLOAD)
			return;
		ThroughPutETC();
	}
	else
		mvTxt->Enabled = false;

	JPRegistry::SetReg("CETC", "mvRadBtn", mvRadBtn->Checked);
}

void Form1::DistanceTxt_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
{
	if (e->KeyCode == ::Keys::Return)
	{
		e->SuppressKeyPress = true;
		ThroughPutETC();
	}		
}

void Form1::RadiusTxt_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
{
	if (e->KeyCode == ::Keys::Return)
	{
		e->SuppressKeyPress = true;
		ThroughPutETC();
	}
}

void Form1::PowerLawAlphaTxt_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
{
	if (e->KeyCode == ::Keys::Return)
	{
		e->SuppressKeyPress = true;
		ThroughPutETC();
	}
}

void Form1::PowerLawNormTxt_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
{
	if (e->KeyCode == ::Keys::Return)
	{
		e->SuppressKeyPress = true;
		ThroughPutETC();
	}
}

void Form1::ExtinctionAvTxt_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
{
	if (e->KeyCode == ::Keys::Return)
	{
		e->SuppressKeyPress = true;
		ThroughPutETC();
	}
}

void Form1::ExtinctionColumnDensityTxt_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
{
	if (e->KeyCode == ::Keys::Return)
	{
		e->SuppressKeyPress = true;
		ThroughPutETC();
	}
}

void Form1::ExtinctionRvTxt_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
{
	if (e->KeyCode == ::Keys::Return)
	{
		e->SuppressKeyPress = true;
		ThroughPutETC();
	}
}

void Form1::mvTxt_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
{
	if (e->KeyCode == ::Keys::Return)
	{
		e->SuppressKeyPress = true;
		ThroughPutETC();
	}
}

void Form1::mTxt_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
{
	if (e->KeyCode == ::Keys::Return)
	{
		e->SuppressKeyPress = true;
		ThroughPutETC();
	}
}

void Form1::RedShiftTxt_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
{
	if (e->KeyCode == ::Keys::Return)
	{
		e->SuppressKeyPress = true;
		ThroughPutETC();
	}
}

void Form1::SourceBlackbodyTempTxt_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
{
	if (e->KeyCode == ::Keys::Return)
	{
		e->SuppressKeyPress = true;
		ThroughPutETC();
	}
}

void Form1::SourceBlackbodyTempTxt_TextChanged(System::Object^  sender, System::EventArgs^  e)
{
	JPRegistry::SetReg("CETC", "SourceBlackbodyTempTxt", SourceBlackbodyTempTxt->Text);
}

void Form1::SourceAGNDrop_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
{
	if (FIRSTSLOAD)
		return;

	JPRegistry::SetReg("CETC", "SourceAGNDropIndex", SourceAGNDrop->SelectedIndex);

	ThroughPutETC();
}

void Form1::SourcePowerLawRadBtn_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
{
	if (SourcePowerLawRadBtn->Checked)
	{
		SourcePowerLawDrop->Enabled = true;
		PowerLawAlphaTxt->Enabled = true;
		PowerLawNormTxt->Enabled = true;
		SourceGalaxyDrop->Enabled = false;
		RedShiftTxt->Enabled = false;
		mTxt->Enabled = false;
		mvRadBtn->Enabled = false;
		mvTxt->Enabled = false;
		DistanceRadBtn->Enabled = false;
		DistanceTxt->Enabled = false;
		RadiusTxt->Enabled = false;
		ISPOINTSOURCE = true;

		if (FIRSTSLOAD)
			return;
		ThroughPutETC();
	}
	else
	{
		SourcePowerLawDrop->Enabled = false;
		PowerLawAlphaTxt->Enabled = false;
		PowerLawNormTxt->Enabled = false;
	}

	JPRegistry::SetReg("CETC", "SourcePowerLawRadBtn", SourcePowerLawRadBtn->Checked);
}

void Form1::SourcePowerLawDrop_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
{
	if (SourcePowerLawDrop->SelectedIndex == 0)
		PowerLawAlphaLabel->Text = "Alpha nu:";
	else
		PowerLawAlphaLabel->Text = "Alpha lambda:";

	if (FIRSTSLOAD)
		return;

	JPRegistry::SetReg("CETC", "SourcePowerLawDropIndex", SourcePowerLawDrop->SelectedIndex);

	ThroughPutETC();
}

void Form1::FilterUpDate()
{
	Chart_Filter->Series[0]->Points->Clear();
	Chart_Final->Series[0]->Points->Clear();
	for (int i = 0; i < LAMBDA_NM->Length; i++)
	{
		Chart_Filter->Series[0]->Points->AddXY(LAMBDA_NM[i], FILTERS[i, SELECTED_FILTER]);
		Chart_Final->Series[0]->Points->AddXY(LAMBDA_NM[i], FINAL_FLUX_FILTERS[i, SELECTED_FILTER]);
	}
}

void Form1::FilterUVDarkRadBtn_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
{
	if (FIRSTSLOAD)
		return;

	JPRegistry::SetReg("CETC", "FilterUVDarkRadBtnChckd", FilterUVDarkRadBtn->Checked);
	
	if (FilterUVDarkRadBtn->Checked)
	{
		SELECTED_FILTER = 0;
		FilterUpDate();
	}
}

void Form1::FilterUVRadBtn_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
{
	if (FIRSTSLOAD)
		return;

	JPRegistry::SetReg("CETC", "FilterUVRadBtnChckd", FilterUVRadBtn->Checked);
	
	if (FilterUVRadBtn->Checked)
	{
		SELECTED_FILTER = 1;
		FilterUpDate();
	}
}

void Form1::FilteruWideRadBtn_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
{
	if (FIRSTSLOAD)
		return;

	JPRegistry::SetReg("CETC", "FilterUWideRadBtnChckd", FilteruWideRadBtn->Checked);
	
	if (FilteruWideRadBtn->Checked)
	{
		SELECTED_FILTER = 2;
		FilterUpDate();
	}
}

void Form1::FilteruRadBtn_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
{
	if (FIRSTSLOAD)
		return;

	JPRegistry::SetReg("CETC", "FilteruRadBtnChckd", FilteruRadBtn->Checked);
	
	if (FilteruRadBtn->Checked)
	{
		SELECTED_FILTER = 3;
		FilterUpDate();
	}
}

void Form1::FiltergRadBtn_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
{
	if (FIRSTSLOAD)
		return;

	JPRegistry::SetReg("CETC", "FiltergRadBtnChckd", FiltergRadBtn->Checked);
	
	if (FiltergRadBtn->Checked)
	{
		SELECTED_FILTER = 4;
		FilterUpDate();
	}
}

void Form1::CheckTextToNum(TextBox^ textbox)
{
	if (textbox->Text == "." || textbox->Text == "")
		return;

	try
	{
		double var = ::Convert::ToDouble(textbox->Text);
		if (var < 0)
			textbox->Text = "0";
	}
	catch (...)
	{
		textbox->Text = "0";
	}
}

void Form1::PowerLawAlphaTxt_TextChanged(System::Object^  sender, System::EventArgs^  e)
{
	CheckTextToNum((TextBox^)sender);
	JPRegistry::SetReg("CETC", "PowerLawAlphaTxt", PowerLawAlphaTxt->Text);
}

void Form1::PowerLawNormTxt_TextChanged(System::Object^  sender, System::EventArgs^  e)
{
	CheckTextToNum((TextBox^)sender);
	JPRegistry::SetReg("CETC", "PowerLawNormTxt", PowerLawNormTxt->Text);
}

void Form1::mTxt_TextChanged(System::Object^  sender, System::EventArgs^  e)
{
	CheckTextToNum((TextBox^)sender);
	JPRegistry::SetReg("CETC", "mTxt", mTxt->Text);
}

void Form1::RedShiftTxt_TextChanged(System::Object^  sender, System::EventArgs^  e)
{
	CheckTextToNum((TextBox^)sender);
	JPRegistry::SetReg("CETC", "RedShiftTxt", RedShiftTxt->Text);
}

void Form1::DistanceTxt_TextChanged(System::Object^  sender, System::EventArgs^  e)
{
	CheckTextToNum((TextBox^)sender);
	JPRegistry::SetReg("CETC", "DistanceTxt", DistanceTxt->Text);
}

void Form1::mvTxt_TextChanged(System::Object^  sender, System::EventArgs^  e)
{
	CheckTextToNum((TextBox^)sender);
	JPRegistry::SetReg("CETC", "mvTxt", mvTxt->Text);
}

void Form1::RadiusTxt_TextChanged(System::Object^  sender, System::EventArgs^  e)
{
	CheckTextToNum((TextBox^)sender);
	JPRegistry::SetReg("CETC", "RadiusTxt", RadiusTxt->Text);
}

void Form1::ExtinctionRvTxt_TextChanged(System::Object^  sender, System::EventArgs^  e)
{
	CheckTextToNum((TextBox^)sender);
	JPRegistry::SetReg("CETC", "ExtinctionRvTxt", ExtinctionRvTxt->Text);
}

void Form1::ExtinctionAvTxt_TextChanged(System::Object^  sender, System::EventArgs^  e)
{
	CheckTextToNum((TextBox^)sender);
	JPRegistry::SetReg("CETC", "ExtinctionAvTxt", ExtinctionAvTxt->Text);
}

void Form1::ExtinctionColumnDensityTxt_TextChanged(System::Object^  sender, System::EventArgs^  e) 
{
	CheckTextToNum((TextBox^)sender);
	JPRegistry::SetReg("CETC", "ExtinctionColumnDensityTxt", ExtinctionColumnDensityTxt->Text);
}

void Form1::ExtinctionAvRadBtn_CheckedChanged(System::Object^  sender, System::EventArgs^  e) 
{
	if (ExtinctionAvRadBtn->Checked)
	{
		ExtinctionAvTxt->Enabled = true;
		ExtinctionColumnDensityTxt->Enabled = false;
	}
	else
		ExtinctionAvTxt->Enabled = false;

	if (FIRSTSLOAD)
		return;

	JPRegistry::SetReg("CETC", "ExtinctionAvRadBtn", ExtinctionAvRadBtn->Checked);

	ThroughPutETC();
}

void Form1::ExtinctionColumnDensityRadBtn_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
{
	if (ExtinctionColumnDensityRadBtn->Checked)
	{
		ExtinctionColumnDensityTxt->Enabled = true;
		ExtinctionAvTxt->Enabled = false;
	}
	else
		ExtinctionColumnDensityTxt->Enabled = false;

	if (FIRSTSLOAD)
		return;

	JPRegistry::SetReg("CETC", "ExtinctionColumnDensityRadBtn", ExtinctionColumnDensityRadBtn->Checked);

	ThroughPutETC();
}

void Form1::ExtinctionDistanceRadBtn_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
{
	if (ExtinctionDistanceRadBtn->Checked)
	{
		ExtinctionAvTxt->Enabled = false;
		ExtinctionColumnDensityTxt->Enabled = false;
	}

	if (FIRSTSLOAD)
		return;

	JPRegistry::SetReg("CETC", "ExtinctionDistanceRadBtn", ExtinctionDistanceRadBtn->Checked);

	ThroughPutETC();
}

void Form1::SourceGalaxyDrop_DropDown(System::Object^  sender, System::EventArgs^  e)
{	
	SourceGalaxyDrop->Width = 310;
}

void Form1::SourceGalaxyDrop_DropDownClosed(System::Object^  sender, System::EventArgs^  e)
{
	SourceGalaxyDrop->Width = 88;
}

void Form1::SourceAGNDrop_DropDown(System::Object^  sender, System::EventArgs^  e)
{
	SourceAGNDrop->Width = 200;
}

void Form1::SourceAGNDrop_DropDownClosed(System::Object^  sender, System::EventArgs^  e)
{
	SourceAGNDrop->Width = 88;
}

void Form1::ExtinctionHelpBtn_Click(System::Object^  sender, System::EventArgs^  e)
{
	String^ helpfile = CASTOR_DIRECTORY + "Help\\HelpExtinction.txt";
	System::IO::StreamReader^ sr = gcnew System::IO::StreamReader(helpfile);
	String^ help = sr->ReadToEnd();
	sr->Close();
	::MessageBox::Show(help, "Extinction Help...");
}

void Form1::SpectralTypeHelpBtn_Click(System::Object^  sender, System::EventArgs^  e)
{
	String^ helpfile = CASTOR_DIRECTORY + "Help\\HelpSpectralType.txt";
	System::IO::StreamReader^ sr = gcnew System::IO::StreamReader(helpfile);
	String^ help = sr->ReadToEnd();
	sr->Close();
	::MessageBox::Show(help, "Spectral Type Help...");
}

void Form1::AGNTypeHelpBtn_Click(System::Object^  sender, System::EventArgs^  e)
{
	String^ helpfile = CASTOR_DIRECTORY + "Help\\HelpAGN.txt";
	System::IO::StreamReader^ sr = gcnew System::IO::StreamReader(helpfile);
	String^ help = sr->ReadToEnd();
	sr->Close();
	::MessageBox::Show(help, "AGN Help...");
}

void Form1::GalaxyTypeHelpBtn_Click(System::Object^  sender, System::EventArgs^  e)
{
	String^ helpfile = CASTOR_DIRECTORY + "Help\\HelpGalaxy.txt";
	System::IO::StreamReader^ sr = gcnew System::IO::StreamReader(helpfile);
	String^ help = sr->ReadToEnd();
	sr->Close();
	::MessageBox::Show(help, "Galaxy Help...");
}

void Form1::SNHelpBtn_Click(System::Object^  sender, System::EventArgs^  e)
{
	String^ helpfile = CASTOR_DIRECTORY + "Help\\HelpSN.txt";
	System::IO::StreamReader^ sr = gcnew System::IO::StreamReader(helpfile);
	String^ help = sr->ReadToEnd();
	sr->Close();
	::MessageBox::Show(help, "SN Help...");
}

void Form1::SNTargetUpD_ValueChanged(System::Object^  sender, System::EventArgs^  e)
{
	if (FIRSTSLOAD)
		return;

	JPRegistry::SetReg("CETC", "SNTargetUpD", SNTargetUpD->Value.ToString());

	if (!COMPUTATION_EXISTS)
		return;

	double SN_target = (double)SNTargetUpD->Value;
	for (int i = 0; i < FILTER_FILENAMES->Length; i++)
		SN_TIME[i] = SN_target * SN_target * (1 + FINAL_COUNTS_BG[i] / FINAL_COUNTS[i]) / FINAL_COUNTS[i];

	//uvdark, uv, uwide, u, g
	SNTimeUVDarkLabel->Text = SN_TIME[0].ToString("e2");
	SNTimeUVLabel->Text = SN_TIME[1].ToString("e2");
	SNTimeuWideLabel->Text = SN_TIME[2].ToString("e2");
	SNTimeuLabel->Text = SN_TIME[3].ToString("e2");
	SNTimegLabel->Text = SN_TIME[4].ToString("e2");
}

void Form1::PlateScaleUpD_ValueChanged(System::Object^  sender, System::EventArgs^  e)
{
	if (FIRSTSLOAD)
		return;

	JPRegistry::SetReg("CETC", "PlateScaleUpD", PlateScaleUpD->Value.ToString());
	ThroughPutETC();
}

void Form1::FWHMUpD_ValueChanged(System::Object^  sender, System::EventArgs^  e)
{
	if (FIRSTSLOAD)
		return;

	JPRegistry::SetReg("CETC", "FWHMUpD", FWHMUpD->Value.ToString());
	ThroughPutETC();
}

double Form1::a_lambda(double lambda)//lambda in nm
{
	double WaveNum = 1.0 / (lambda / 1000); //Wave Number in units of inverse micrometers
	double y, Fa;

	if (WaveNum <3.3)
	{
		y = WaveNum - 1.82;
		return (1 + 0.17699*y - 0.50447*y*y - 0.02427*y*y*y + 0.72085*Math::Pow(y, 4) + 0.01979*Math::Pow(y, 5) - 0.77530*Math::Pow(y, 6) + 0.32999*Math::Pow(y, 7));
	}
	if (WaveNum >= 3.3 && WaveNum < 5.9)
	{
		Fa = 0.0;
	}
	else
	{
		y = WaveNum - 5.9;
		Fa = -0.04473*y*y - 0.009779*y*y*y;
	}
	y = WaveNum - 4.67;
	return (1.752 - 0.316*WaveNum - 0.104 / (y*y + 0.341) + Fa);
}

double Form1::b_lambda(double lambda)//lambda in nm
{
	double WaveNum = 1.0 / (lambda / 1000); //Wave Number in units of inverse micrometers
	double y, Fb;

	if (WaveNum <= 3.3)
	{
		y = WaveNum - 1.82;
		return (1.41338*y + 2.28305*y*y + 1.07233*y*y*y - 5.38434*Math::Pow(y, 4) - 0.62251*Math::Pow(y, 5) + 5.30260*Math::Pow(y, 6) - 2.09002*Math::Pow(y, 7));
	}
	if (WaveNum > 3.3 && WaveNum < 5.9)
	{
		Fb = 0.0;
	}
	else
	{
		y = WaveNum - 5.9;
		Fb = 0.2130*y*y + 0.1207*y*y*y;
	}
	y = WaveNum - 4.62;
	return (-3.090 + 1.825*WaveNum + 1.206 / (y*y + 0.263) + Fb);
}

void Form1::SourceBlackBody()
{
	double distance = ::Convert::ToDouble(DistanceTxt->Text);
	double radius = Convert::ToDouble(RadiusTxt->Text);
	double solidangle = Math::PI * (radius*SR2KPC)*(radius*SR2KPC) / (distance*distance);
	double BB_temp = Convert::ToDouble(SourceBlackbodyTempTxt->Text->Substring(0, 7));
	double lambda;

	for (int i = 0; i < NELEMENTS; i++)
	{
		lambda = LAMBDA_NM[i] * 1e-9; //lambda in meters 
		SOURCE_FLUX[i] = 2 * h*c*c*Math::Pow(lambda, -5)*Math::Pow(Math::Exp((h*c) / (lambda*BB_temp*K)) - 1, -1);
		SOURCE_FLUX_LOCAL[i] = SOURCE_FLUX[i] * solidangle;
	}
}

void Form1::SourcePowerLaw()
{
	double alpha = Convert::ToDouble(PowerLawAlphaTxt->Text);
	double norm = Convert::ToDouble(PowerLawNormTxt->Text);
	double lambda;

	if (SourcePowerLawDrop->SelectedIndex == 0)//f(nu)
	{
		for (int i = 0; i < NELEMENTS; i++)
		{
			lambda = LAMBDA_NM[i] * 1e-9; //lambda in meters
			SOURCE_FLUX[i] = c * norm*1e-10*(4.13322e-20 / (lambda*lambda))*Math::Pow(lambda / 1.2398e-9, alpha);
			SOURCE_FLUX_LOCAL[i] = SOURCE_FLUX[i];
		}
	}

	if (SourcePowerLawDrop->SelectedIndex == 1)//f(lambda)
	{
		for (int i = 0; i < NELEMENTS; i++)
		{
			lambda = LAMBDA_NM[i] * 1e-9; //lambda in meters 
			SOURCE_FLUX[i] = norm * 1e-13*1e7*Math::Pow(lambda / 300e-9, -alpha);
			SOURCE_FLUX_LOCAL[i] = SOURCE_FLUX[i];
		}
	}
}

void Form1::SourceSpectralType()
{
	String^ spectype = (String^)SourceStarDrop->SelectedItem;
	array<double>^ kurucz_lambda;
	array<double>^ kurucz_source;
	String^ Kuruczfile;

	if (spectype == "O5V")
	{
		Kuruczfile = CASTOR_DIRECTORY + "SpectralType\\kp00_45000.fits";
		kurucz_lambda = JPFits::FitsFile::ReadBinaryTableExtensionEntry(Kuruczfile, "", "WAVELENGTH");
		kurucz_source = JPFits::FitsFile::ReadBinaryTableExtensionEntry(Kuruczfile, "", "g50");
		RadiusTxt->Text = "7.4";
		FV_SURFACE = 7.2463e8;
	}

	if (spectype == "O6V")
	{
		Kuruczfile = CASTOR_DIRECTORY + "SpectralType\\kp00_40000.fits";
		kurucz_lambda = JPFits::FitsFile::ReadBinaryTableExtensionEntry(Kuruczfile, "", "WAVELENGTH");
		kurucz_source = JPFits::FitsFile::ReadBinaryTableExtensionEntry(Kuruczfile, "", "g45");
		RadiusTxt->Text = "7.4";
		FV_SURFACE = 6.3521E8;
	}

	if (spectype == "O8V")
	{
		Kuruczfile = CASTOR_DIRECTORY + "SpectralType\\kp00_35000.fits";
		kurucz_lambda = JPFits::FitsFile::ReadBinaryTableExtensionEntry(Kuruczfile, "", "WAVELENGTH");
		kurucz_source = JPFits::FitsFile::ReadBinaryTableExtensionEntry(Kuruczfile, "", "g40");
		RadiusTxt->Text = "7.4";
		FV_SURFACE = 5.3639E8;
	}

	if (spectype == "B0V")
	{
		Kuruczfile = CASTOR_DIRECTORY + "SpectralType\\kp00_30000.fits";
		kurucz_lambda = JPFits::FitsFile::ReadBinaryTableExtensionEntry(Kuruczfile, "", "WAVELENGTH");
		kurucz_source = JPFits::FitsFile::ReadBinaryTableExtensionEntry(Kuruczfile, "", "g40");
		RadiusTxt->Text = "6.64";
		FV_SURFACE = 4.0320E8;
	}

	if (spectype == "B3V")
	{
		Kuruczfile = CASTOR_DIRECTORY + "SpectralType\\kp00_19000.fits";
		kurucz_lambda = JPFits::FitsFile::ReadBinaryTableExtensionEntry(Kuruczfile, "", "WAVELENGTH");
		kurucz_source = JPFits::FitsFile::ReadBinaryTableExtensionEntry(Kuruczfile, "", "g40");
		RadiusTxt->Text = "3.25";
		FV_SURFACE = 1.7375E8;
	}

	if (spectype == "B5V")
	{
		Kuruczfile = CASTOR_DIRECTORY + "SpectralType\\kp00_15000.fits";
		kurucz_lambda = JPFits::FitsFile::ReadBinaryTableExtensionEntry(Kuruczfile, "", "WAVELENGTH");
		kurucz_source = JPFits::FitsFile::ReadBinaryTableExtensionEntry(Kuruczfile, "", "g40");
		RadiusTxt->Text = "2.74";
		FV_SURFACE = 1.1849E8;
	}

	if (spectype == "B8V")
	{
		Kuruczfile = CASTOR_DIRECTORY + "SpectralType\\kp00_12000.fits";
		kurucz_lambda = JPFits::FitsFile::ReadBinaryTableExtensionEntry(Kuruczfile, "", "WAVELENGTH");
		kurucz_source = JPFits::FitsFile::ReadBinaryTableExtensionEntry(Kuruczfile, "", "g40");
		RadiusTxt->Text = "2.23";
		FV_SURFACE = 8.0542E7;
	}

	if (spectype == "A0V")
	{
		Kuruczfile = CASTOR_DIRECTORY + "SpectralType\\kp00_9500.fits";
		kurucz_lambda = JPFits::FitsFile::ReadBinaryTableExtensionEntry(Kuruczfile, "", "WAVELENGTH");
		kurucz_source = JPFits::FitsFile::ReadBinaryTableExtensionEntry(Kuruczfile, "", "g40");
		RadiusTxt->Text = "1.87";
		FV_SURFACE = 4.9985E7;
	}

	if (spectype == "A5V")
	{
		Kuruczfile = CASTOR_DIRECTORY + "SpectralType\\kp00_8250.fits";
		kurucz_lambda = JPFits::FitsFile::ReadBinaryTableExtensionEntry(Kuruczfile, "", "WAVELENGTH");
		kurucz_source = JPFits::FitsFile::ReadBinaryTableExtensionEntry(Kuruczfile, "", "g45");
		RadiusTxt->Text = "1.69";
		FV_SURFACE = 3.3059E7;
	}

	if (spectype == "F0V")
	{
		Kuruczfile = CASTOR_DIRECTORY + "SpectralType\\kp00_7250.fits";
		kurucz_lambda = JPFits::FitsFile::ReadBinaryTableExtensionEntry(Kuruczfile, "", "WAVELENGTH");
		kurucz_source = JPFits::FitsFile::ReadBinaryTableExtensionEntry(Kuruczfile, "", "g45");
		RadiusTxt->Text = "1.51";
		FV_SURFACE = 1.9702E7;
	}

	if (spectype == "F5V")
	{
		Kuruczfile = CASTOR_DIRECTORY + "SpectralType\\kp00_6500.fits";
		kurucz_lambda = JPFits::FitsFile::ReadBinaryTableExtensionEntry(Kuruczfile, "", "WAVELENGTH");
		kurucz_source = JPFits::FitsFile::ReadBinaryTableExtensionEntry(Kuruczfile, "", "g45");
		RadiusTxt->Text = "1.32";
		FV_SURFACE = 1.2481E7;
	}

	if (spectype == "G0V")
	{
		Kuruczfile = CASTOR_DIRECTORY + "SpectralType\\kp00_6000.fits";
		kurucz_lambda = JPFits::FitsFile::ReadBinaryTableExtensionEntry(Kuruczfile, "", "WAVELENGTH");
		kurucz_source = JPFits::FitsFile::ReadBinaryTableExtensionEntry(Kuruczfile, "", "g45");
		RadiusTxt->Text = "1.08";
		FV_SURFACE = 8.7620E6;
	}

	if (spectype == "G5V")
	{
		Kuruczfile = CASTOR_DIRECTORY + "SpectralType\\kp00_5750.fits";
		kurucz_lambda = JPFits::FitsFile::ReadBinaryTableExtensionEntry(Kuruczfile, "", "WAVELENGTH");
		kurucz_source = JPFits::FitsFile::ReadBinaryTableExtensionEntry(Kuruczfile, "", "g45");
		RadiusTxt->Text = "0.96";
		FV_SURFACE = 7.1766E6;
	}

	if (spectype == "K0V")
	{
		Kuruczfile = CASTOR_DIRECTORY + "SpectralType\\kp00_5250.fits";
		kurucz_lambda = JPFits::FitsFile::ReadBinaryTableExtensionEntry(Kuruczfile, "", "WAVELENGTH");
		kurucz_source = JPFits::FitsFile::ReadBinaryTableExtensionEntry(Kuruczfile, "", "g45");
		RadiusTxt->Text = "0.81";
		FV_SURFACE = 4.4845E6;
	}

	if (spectype == "K5V")
	{
		Kuruczfile = CASTOR_DIRECTORY + "SpectralType\\kp00_4250.fits";
		kurucz_lambda = JPFits::FitsFile::ReadBinaryTableExtensionEntry(Kuruczfile, "", "WAVELENGTH");
		kurucz_source = JPFits::FitsFile::ReadBinaryTableExtensionEntry(Kuruczfile, "", "g45");
		RadiusTxt->Text = "0.68";
		FV_SURFACE = 1.1103E6;
	}

	if (spectype == "M0V")
	{
		Kuruczfile = CASTOR_DIRECTORY + "SpectralType\\kp00_3750.fits";
		kurucz_lambda = JPFits::FitsFile::ReadBinaryTableExtensionEntry(Kuruczfile, "", "WAVELENGTH");
		kurucz_source = JPFits::FitsFile::ReadBinaryTableExtensionEntry(Kuruczfile, "", "g45");
		RadiusTxt->Text = "0.61";
		FV_SURFACE = 4.3687E5;
	}

	if (spectype == "M2V")
	{
		Kuruczfile = CASTOR_DIRECTORY + "SpectralType\\kp00_3500.fits";
		kurucz_lambda = JPFits::FitsFile::ReadBinaryTableExtensionEntry(Kuruczfile, "", "WAVELENGTH");
		kurucz_source = JPFits::FitsFile::ReadBinaryTableExtensionEntry(Kuruczfile, "", "g45");
		RadiusTxt->Text = "0.55";
		FV_SURFACE = 2.3601E5;
	}

	if (spectype == "M5V")
	{
		Kuruczfile = CASTOR_DIRECTORY + "SpectralType\\kp00_3500.fits";
		kurucz_lambda = JPFits::FitsFile::ReadBinaryTableExtensionEntry(Kuruczfile, "", "WAVELENGTH");
		kurucz_source = JPFits::FitsFile::ReadBinaryTableExtensionEntry(Kuruczfile, "", "g50");
		RadiusTxt->Text = "0.49";
		FV_SURFACE = 2.4302E5;
	}

	if (spectype == "B0III")
	{
		Kuruczfile = CASTOR_DIRECTORY + "SpectralType\\kp00_29000.fits";
		kurucz_lambda = JPFits::FitsFile::ReadBinaryTableExtensionEntry(Kuruczfile, "", "WAVELENGTH");
		kurucz_source = JPFits::FitsFile::ReadBinaryTableExtensionEntry(Kuruczfile, "", "g35");
		RadiusTxt->Text = "3.0";
		FV_SURFACE = 3.9357E8;
	}

	if (spectype == "B5III")
	{
		Kuruczfile = CASTOR_DIRECTORY + "SpectralType\\kp00_15000.fits";
		kurucz_lambda = JPFits::FitsFile::ReadBinaryTableExtensionEntry(Kuruczfile, "", "WAVELENGTH");
		kurucz_source = JPFits::FitsFile::ReadBinaryTableExtensionEntry(Kuruczfile, "", "g35");
		RadiusTxt->Text = "3.0";
		FV_SURFACE = 1.1880E8;
	}

	if (spectype == "G0III")
	{
		Kuruczfile = CASTOR_DIRECTORY + "SpectralType\\kp00_5750.fits";
		kurucz_lambda = JPFits::FitsFile::ReadBinaryTableExtensionEntry(Kuruczfile, "", "WAVELENGTH");
		kurucz_source = JPFits::FitsFile::ReadBinaryTableExtensionEntry(Kuruczfile, "", "g30");
		RadiusTxt->Text = "5.0";
		FV_SURFACE = 7.2888E6;
	}

	if (spectype == "G5III")
	{
		Kuruczfile = CASTOR_DIRECTORY + "SpectralType\\kp00_5250.fits";
		kurucz_lambda = JPFits::FitsFile::ReadBinaryTableExtensionEntry(Kuruczfile, "", "WAVELENGTH");
		kurucz_source = JPFits::FitsFile::ReadBinaryTableExtensionEntry(Kuruczfile, "", "g25");
		RadiusTxt->Text = "8.0";
		FV_SURFACE = 4.6069E6;
	}

	if (spectype == "K0III")
	{
		Kuruczfile = CASTOR_DIRECTORY + "SpectralType\\kp00_4750.fits";
		kurucz_lambda = JPFits::FitsFile::ReadBinaryTableExtensionEntry(Kuruczfile, "", "WAVELENGTH");
		kurucz_source = JPFits::FitsFile::ReadBinaryTableExtensionEntry(Kuruczfile, "", "g20");
		RadiusTxt->Text = "11.0";
		FV_SURFACE = 2.5875E6;
	}

	if (spectype == "K5III")
	{
		Kuruczfile = CASTOR_DIRECTORY + "SpectralType\\kp00_4000.fits";
		kurucz_lambda = JPFits::FitsFile::ReadBinaryTableExtensionEntry(Kuruczfile, "", "WAVELENGTH");
		kurucz_source = JPFits::FitsFile::ReadBinaryTableExtensionEntry(Kuruczfile, "", "g15");
		RadiusTxt->Text = "28.0";
		FV_SURFACE = 7.3310E5;
	}

	if (spectype == "M0III")
	{
		Kuruczfile = CASTOR_DIRECTORY + "SpectralType\\kp00_3750.fits";
		kurucz_lambda = JPFits::FitsFile::ReadBinaryTableExtensionEntry(Kuruczfile, "", "WAVELENGTH");
		kurucz_source = JPFits::FitsFile::ReadBinaryTableExtensionEntry(Kuruczfile, "", "g15");
		RadiusTxt->Text = "28.0";
		FV_SURFACE = 4.0315E5;
	}

	if (spectype == "O5I")
	{
		Kuruczfile = CASTOR_DIRECTORY + "SpectralType\\kp00_40000.fits";
		kurucz_lambda = JPFits::FitsFile::ReadBinaryTableExtensionEntry(Kuruczfile, "", "WAVELENGTH");
		kurucz_source = JPFits::FitsFile::ReadBinaryTableExtensionEntry(Kuruczfile, "", "g45");
		RadiusTxt->Text = "60.0";
		FV_SURFACE = 6.3521E8;
	}

	if (spectype == "O6I")
	{
		Kuruczfile = CASTOR_DIRECTORY + "SpectralType\\kp00_40000.fits";
		kurucz_lambda = JPFits::FitsFile::ReadBinaryTableExtensionEntry(Kuruczfile, "", "WAVELENGTH");
		kurucz_source = JPFits::FitsFile::ReadBinaryTableExtensionEntry(Kuruczfile, "", "g45");
		RadiusTxt->Text = "60.0";
		FV_SURFACE = 6.3521E8;
	}

	if (spectype == "O8I")
	{
		Kuruczfile = CASTOR_DIRECTORY + "SpectralType\\kp00_34000.fits";
		kurucz_lambda = JPFits::FitsFile::ReadBinaryTableExtensionEntry(Kuruczfile, "", "WAVELENGTH");
		kurucz_source = JPFits::FitsFile::ReadBinaryTableExtensionEntry(Kuruczfile, "", "g40");
		RadiusTxt->Text = "60.0";
		FV_SURFACE = 5.1313E8;
	}

	if (spectype == "B0I")
	{
		Kuruczfile = CASTOR_DIRECTORY + "SpectralType\\kp00_26000.fits";
		kurucz_lambda = JPFits::FitsFile::ReadBinaryTableExtensionEntry(Kuruczfile, "", "WAVELENGTH");
		kurucz_source = JPFits::FitsFile::ReadBinaryTableExtensionEntry(Kuruczfile, "", "g30");
		RadiusTxt->Text = "60.0";
		FV_SURFACE = 3.2939E8;
	}

	if (spectype == "B5I")
	{
		Kuruczfile = CASTOR_DIRECTORY + "SpectralType\\kp00_14000.fits";
		kurucz_lambda = JPFits::FitsFile::ReadBinaryTableExtensionEntry(Kuruczfile, "", "WAVELENGTH");
		kurucz_source = JPFits::FitsFile::ReadBinaryTableExtensionEntry(Kuruczfile, "", "g25");
		RadiusTxt->Text = "60.0";
		FV_SURFACE = 1.0584E8;
	}

	if (spectype == "A0I")
	{
		Kuruczfile = CASTOR_DIRECTORY + "SpectralType\\kp00_9750.fits";
		kurucz_lambda = JPFits::FitsFile::ReadBinaryTableExtensionEntry(Kuruczfile, "", "WAVELENGTH");
		kurucz_source = JPFits::FitsFile::ReadBinaryTableExtensionEntry(Kuruczfile, "", "g20");
		RadiusTxt->Text = "60.0";
		FV_SURFACE = 5.2038E7;
	}

	if (spectype == "A5I")
	{
		Kuruczfile = CASTOR_DIRECTORY + "SpectralType\\kp00_8500.fits";
		kurucz_lambda = JPFits::FitsFile::ReadBinaryTableExtensionEntry(Kuruczfile, "", "WAVELENGTH");
		kurucz_source = JPFits::FitsFile::ReadBinaryTableExtensionEntry(Kuruczfile, "", "g20");
		RadiusTxt->Text = "60.0";
		FV_SURFACE = 3.7259E7;
	}

	if (spectype == "F0I")
	{
		Kuruczfile = CASTOR_DIRECTORY + "SpectralType\\kp00_7750.fits";
		kurucz_lambda = JPFits::FitsFile::ReadBinaryTableExtensionEntry(Kuruczfile, "", "WAVELENGTH");
		kurucz_source = JPFits::FitsFile::ReadBinaryTableExtensionEntry(Kuruczfile, "", "g20");
		RadiusTxt->Text = "60.0";
		FV_SURFACE = 2.7696E7;
	}

	if (spectype == "F5I")
	{
		Kuruczfile = CASTOR_DIRECTORY + "SpectralType\\kp00_7000.fits";
		kurucz_lambda = JPFits::FitsFile::ReadBinaryTableExtensionEntry(Kuruczfile, "", "WAVELENGTH");
		kurucz_source = JPFits::FitsFile::ReadBinaryTableExtensionEntry(Kuruczfile, "", "g15");
		RadiusTxt->Text = "60.0";
		FV_SURFACE = 1.8639E7;
	}

	if (spectype == "G0I")
	{
		Kuruczfile = CASTOR_DIRECTORY + "SpectralType\\kp00_5500.fits";
		kurucz_lambda = JPFits::FitsFile::ReadBinaryTableExtensionEntry(Kuruczfile, "", "WAVELENGTH");
		kurucz_source = JPFits::FitsFile::ReadBinaryTableExtensionEntry(Kuruczfile, "", "g15");
		RadiusTxt->Text = "75.0";
		FV_SURFACE = 5.9486E6;
	}

	if (spectype == "G5I")
	{
		Kuruczfile = CASTOR_DIRECTORY + "SpectralType\\kp00_4750.fits";
		kurucz_lambda = JPFits::FitsFile::ReadBinaryTableExtensionEntry(Kuruczfile, "", "WAVELENGTH");
		kurucz_source = JPFits::FitsFile::ReadBinaryTableExtensionEntry(Kuruczfile, "", "g10");
		RadiusTxt->Text = "95.0";
		FV_SURFACE = 2.6013E6;
	}

	if (spectype == "K0I")
	{
		Kuruczfile = CASTOR_DIRECTORY + "SpectralType\\kp00_4500.fits";
		kurucz_lambda = JPFits::FitsFile::ReadBinaryTableExtensionEntry(Kuruczfile, "", "WAVELENGTH");
		kurucz_source = JPFits::FitsFile::ReadBinaryTableExtensionEntry(Kuruczfile, "", "g10");
		RadiusTxt->Text = "110.0";
		FV_SURFACE = 1.8243E6;
	}

	if (spectype == "K5I")
	{
		Kuruczfile = CASTOR_DIRECTORY + "SpectralType\\kp00_3750.fits";
		kurucz_lambda = JPFits::FitsFile::ReadBinaryTableExtensionEntry(Kuruczfile, "", "WAVELENGTH");
		kurucz_source = JPFits::FitsFile::ReadBinaryTableExtensionEntry(Kuruczfile, "", "g05");
		RadiusTxt->Text = "150.0";
		FV_SURFACE = 4.0596E5;
	}

	if (spectype == "M0I")
	{
		Kuruczfile = CASTOR_DIRECTORY + "SpectralType\\kp00_3750.fits";
		kurucz_lambda = JPFits::FitsFile::ReadBinaryTableExtensionEntry(Kuruczfile, "", "WAVELENGTH");
		kurucz_source = JPFits::FitsFile::ReadBinaryTableExtensionEntry(Kuruczfile, "", "g00");
		RadiusTxt->Text = "150.0";
		FV_SURFACE = 4.0151E5;
	}

	if (spectype == "M2I")
	{
		Kuruczfile = CASTOR_DIRECTORY + "SpectralType\\kp00_3500.fits";
		kurucz_lambda = JPFits::FitsFile::ReadBinaryTableExtensionEntry(Kuruczfile, "", "WAVELENGTH");
		kurucz_source = JPFits::FitsFile::ReadBinaryTableExtensionEntry(Kuruczfile, "", "g00");
		RadiusTxt->Text = "150.0";
		FV_SURFACE = 1.8380E5;
	}

	//Kurucz profiles are tabulated in Angstroms - source vs. lambda in Angstroms
	//therefore need to resample at nm increments from 120 to 1100
	int i_LAMBDA_NM = 0;
	for (int i = 0; i < kurucz_lambda->Length; i++)
		if (kurucz_lambda[i]/10 > LAMBDA_NM[i_LAMBDA_NM])
		{
			SOURCE_FLUX[i_LAMBDA_NM] = kurucz_source[i] * 1e7;  //convert from erg/s/cm^3 -> J/s/M^3;
			i_LAMBDA_NM++;
			
			if (i_LAMBDA_NM == NELEMENTS)
				break;
		}

	double distance = ::Convert::ToDouble(DistanceTxt->Text);
	double radius = Convert::ToDouble(RadiusTxt->Text);
	double solidangle = Math::PI * (radius*SR2KPC)*(radius*SR2KPC) / (distance*distance);

	if (mvRadBtn->Checked)
	{
		double magnitude = Convert::ToDouble(mvTxt->Text);
		radius = Convert::ToDouble(RadiusTxt->Text);
		distance = Math::Sqrt(Math::Pow(10, 0.4*magnitude)*Math::Pow(3.085680E17 / 1.87, 2) / 4.9885E7*Math::Pow(radius, 2)*FV_SURFACE) / KPC2M;
		DistanceTxt->Text = Math::Round(distance, 3).ToString();
		if (distance < 0.001)
			DistanceTxt->Text = "0.001";
	}

	solidangle = Math::PI * (radius*SR2KPC)*(radius*SR2KPC) / (distance*distance);

	for (int i = 0; i < NELEMENTS; i++)
		SOURCE_FLUX_LOCAL[i] = solidangle * SOURCE_FLUX[i] / Math::PI;// divide by PI because spectrum input is surface flux not intensity: surface flux = PI*intensity
}

void Form1::SourceGalaxy()
{
	String^ galtype = (String^)SourceGalaxyDrop->SelectedItem;
	array<double>^ gal_lambda;
	array<double>^ gal_source;
	String^ galfile;

	if (galtype == "Bulge (norm: v=12.5)")
	{
		galfile = CASTOR_DIRECTORY + "Galaxies\\bulge_template.fits";
		NORM_MAG = 12.5;
	}

	if (galtype == "Elliptical (norm: v=12.5)")
	{
		galfile = CASTOR_DIRECTORY + "Galaxies\\elliptical_template.fits";
		NORM_MAG = 12.5;
	}

	if (galtype == "S0 (norm: v=12.5)")
	{
		galfile = CASTOR_DIRECTORY + "Galaxies\\s0_template.fits";
		NORM_MAG = 12.5;
	}

	if (galtype == "Sa (norm: v=12.5)")
	{
		galfile = CASTOR_DIRECTORY + "Galaxies\\sa_template.fits";
		NORM_MAG = 12.5;
	}

	if (galtype == "Sb (norm: v=12.5)")
	{
		galfile = CASTOR_DIRECTORY + "Galaxies\\sb_template.fits";
		NORM_MAG = 12.5;
	}

	if (galtype == "Sc (norm: v=12.5)")
	{
		galfile = CASTOR_DIRECTORY + "Galaxies\\sc_template.fits";
		NORM_MAG = 12.5;
	}

	if (galtype == "Starburst1: e(b-v)<0.1 (norm: v=12.5)")
	{
		galfile = CASTOR_DIRECTORY + "Galaxies\\starb1_template.fits";
		NORM_MAG = 12.5;
	}

	if (galtype == "Starburst2: 0.11< e(b-v)<0.21 (norm: v=12.5)")
	{
		galfile = CASTOR_DIRECTORY + "Galaxies\\starb2_template.fits";
		NORM_MAG = 12.5;
	}

	if (galtype == "Starburst3: 0.25< e(b-v)<0.35 (norm: v=12.5)")
	{
		galfile = CASTOR_DIRECTORY + "Galaxies\\starb3_template.fits";
		NORM_MAG = 12.5;
	}

	if (galtype == "Starburst4: 0.39< e(b-v)<0.50 (norm: v=12.5)")
	{
		galfile = CASTOR_DIRECTORY + "Galaxies\\starb4_template.fits";
		NORM_MAG = 12.5;
	}

	if (galtype == "Starburst5: 0.51< e(b-v)<0.60 (norm: v=12.5)")
	{
		galfile = CASTOR_DIRECTORY + "Galaxies\\starb5_template.fits";
		NORM_MAG = 12.5;
	}

	if (galtype == "Starburst6: 0.61< e(b-v)<0.70 (norm: v=12.5)")
	{
		galfile = CASTOR_DIRECTORY + "Galaxies\\starb6_template.fits";
		NORM_MAG = 12.5;
	}

	gal_lambda = JPFits::FitsFile::ReadBinaryTableExtensionEntry(galfile, "", "WAVELENGTH");
	gal_source = JPFits::FitsFile::ReadBinaryTableExtensionEntry(galfile, "", "FLUX");
	double redshift = Convert::ToDouble(RedShiftTxt->Text);
	double m = Convert::ToDouble(mTxt->Text);

	//Galaxy profiles are tabulated in Angstroms - source vs. lambda in Angstroms
	//therefore need to resample at nm increments from 120 to 1100
	int i_LAMBDA_NM = 0;
	for (int i = 0; i < gal_lambda->Length; i++)
		if (gal_lambda[i] / 10 > LAMBDA_NM[i_LAMBDA_NM])
		{
			SOURCE_FLUX[i_LAMBDA_NM] = gal_source[i] * 1e7;//convert from erg/s/cm^3 -> J/s/M^3;
			i_LAMBDA_NM++;

			if (i_LAMBDA_NM == NELEMENTS)
				break;
		}

	//Now shift the spectrum for the given red shift to get local source flux
	for (int i = 0; i < gal_lambda->Length; i++)
		gal_lambda[i] = gal_lambda[i] * (1 + redshift);

	//Galaxy profiles are tabulated in Angstroms - source vs. lambda in Angstroms
	//therefore need to resample at nm increments from 120 to 1100
	i_LAMBDA_NM = 0;
	for (int i = 0; i < gal_lambda->Length; i++)
		if (gal_lambda[i] / 10 > LAMBDA_NM[i_LAMBDA_NM])
		{
			SOURCE_FLUX_LOCAL[i_LAMBDA_NM] = gal_source[i] * 1e7;//convert from erg/s/cm^3 -> J/s/M^3;
			i_LAMBDA_NM++;

			if (i_LAMBDA_NM == NELEMENTS)
				break;
		}

	//now redshift the local source flux
	for (int i = 0; i < NELEMENTS; i++)
		SOURCE_FLUX_LOCAL[i] = (SOURCE_FLUX_LOCAL[i] / (1 + redshift)) * (Math::Pow(10, -0.4*(m - NORM_MAG)) / (1 + redshift));

	//double g = tgamma(5);

}

void Form1::SourceAGN()
{
	String^ AGNtype = (String^)SourceAGNDrop->SelectedItem;
	array<double>^ AGN_lambda;
	array<double>^ AGN_source;
	String^ AGNfile;

	if (AGNtype == "Liner (norm: v=12.5)")
	{
		AGNfile = CASTOR_DIRECTORY + "AGN\\liner_template.fits";
		NORM_MAG = 12.5;
	}

	if (AGNtype == "NGC-1068 (norm: v=10.3)")
	{
		AGNfile = CASTOR_DIRECTORY + "AGN\\ngc1068_template.fits";
		NORM_MAG = 10.3;
	}

	if (AGNtype == "QSO (norm: b=12.5)")
	{
		AGNfile = CASTOR_DIRECTORY + "AGN\\qso_template.fits";
		NORM_MAG = 12.5;
	}

	if (AGNtype == "Seyfert1 (norm: b=12.5)")
	{
		AGNfile = CASTOR_DIRECTORY + "AGN\\seyfert1_template.fits";
		NORM_MAG = 12.5;
	}

	if (AGNtype == "Seyfert2 (norm: v=12.5)")
	{
		AGNfile = CASTOR_DIRECTORY + "AGN\\seyfert2_template.fits";
		NORM_MAG = 12.5;
	}

	AGN_lambda = JPFits::FitsFile::ReadBinaryTableExtensionEntry(AGNfile, "", "WAVELENGTH");
	AGN_source = JPFits::FitsFile::ReadBinaryTableExtensionEntry(AGNfile, "", "FLUX");
	double redshift = Convert::ToDouble(RedShiftTxt->Text);
	double m = Convert::ToDouble(mTxt->Text);

	//Galaxy profiles are tabulated in Angstroms - source vs. lambda in Angstroms
	//therefore need to resample at nm increments from 120 to 1100
	int i_LAMBDA_NM = 0;
	for (int i = 0; i < AGN_lambda->Length; i++)
		if (AGN_lambda[i] / 10 > LAMBDA_NM[i_LAMBDA_NM])
		{
			SOURCE_FLUX[i_LAMBDA_NM] = AGN_source[i] * 1e7;  //convert from erg/s/cm^3 -> J/s/M^3;
			i_LAMBDA_NM++;

			if (i_LAMBDA_NM == NELEMENTS)
				break;
		}

	//Now shift the spectrum for the given red shift to get local source flux
	for (int i = 0; i < AGN_lambda->Length; i++)
		AGN_lambda[i] = AGN_lambda[i] * (1 + redshift);

	//Galaxy profiles are tabulated in Angstroms - source vs. lambda in Angstroms
	//therefore need to resample at nm increments from 120 to 1100
	i_LAMBDA_NM = 0;
	for (int i = 0; i < AGN_lambda->Length; i++)
		if (AGN_lambda[i] / 10 > LAMBDA_NM[i_LAMBDA_NM])
		{
			SOURCE_FLUX_LOCAL[i_LAMBDA_NM] = AGN_source[i] * 1e7;  //convert from erg/s/cm^3 -> J/s/M^3;
			i_LAMBDA_NM++;

			if (i_LAMBDA_NM == NELEMENTS)
				break;
		}

	//now redshift the local source flux
	for (int i = 0; i < NELEMENTS; i++)
		SOURCE_FLUX_LOCAL[i] = (SOURCE_FLUX_LOCAL[i] / (1 + redshift)) * (Math::Pow(10, -0.4*(m - NORM_MAG)) / (1 + redshift));
}


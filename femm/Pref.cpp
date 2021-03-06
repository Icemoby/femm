// Pref.cpp : implementation file
//

#include "stdafx.h"
#include "femm.h"
#include <afxdlgs.h>
#include "Pref.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define SelColor	clist[0]
#define MeshColor	clist[1]
#define BlockColor	clist[2]
#define LineColor	clist[3]
#define	GridColor	clist[4]
#define NodeColor	clist[5]
#define	BackColor	clist[6]
#define NameColor	clist[7]

/////////////////////////////////////////////////////////////////////////////
// CPref dialog


CPref::CPref(CWnd* pParent /*=NULL*/)
	: CDialog(CPref::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPref)
	m_d_freq = 0.0;
	m_d_gridsize = 0.25;
	m_d_pixels = 100.0;
	m_d_prec = 1.e-8;
	m_d_minangle = DEFAULT_MINIMUM_ANGLE;
	m_d_depth = 1.0;
	m_d_showgrid = TRUE;
	m_d_snapgrid = FALSE;
	m_d_showorigin = FALSE;
	m_d_shownames = TRUE;
	//}}AFX_DATA_INIT
	
	s_action=0;
	s_coord=0;
	s_length=0;
	s_type=0;
	s_solver=0;

	clist=(COLORREF *)calloc(16,sizeof(COLORREF));
	clist[0]= dSelColor;
	clist[1]= dMeshColor;
	clist[2]= dBlockColor;
	clist[3]= dLineColor;
	clist[4]= dGridColor;
	clist[5]= dNodeColor;
	clist[6]= dBackColor;
	clist[7]= dNameColor;
}

CPref::~CPref()
{
	free(clist);
}


void CPref::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPref)
	DDX_Control(pDX, IDC_DSOLVER, m_d_solver);
	DDX_Control(pDX, IDC_DCOLOR, m_d_color);
	DDX_Control(pDX, IDC_DTYPE, m_d_type);
	DDX_Control(pDX, IDC_DLENGTH, m_d_length);
	DDX_Control(pDX, IDC_DCOORD, m_d_coord);
	DDX_Control(pDX, IDC_DACTION, m_d_action);
	DDX_Text(pDX, IDC_DFREQ, m_d_freq);
	DDX_Text(pDX, IDC_DGRIDSIZE, m_d_gridsize);
	DDX_Text(pDX, IDC_DPIXELS, m_d_pixels);
	DDX_Text(pDX, IDC_DDEPTH, m_d_depth);
	DDX_Text(pDX, IDC_DPREC, m_d_prec);
	DDV_MinMaxDouble(pDX, m_d_prec, 1.e-016, 1.e-008);
	DDX_Text(pDX, IDC_DMINANGLE, m_d_minangle);
	DDV_MinMaxDouble(pDX, m_d_minangle, 1., 33.8);
	DDX_Check(pDX, IDC_DSHOWGRID, m_d_showgrid);
	DDX_Check(pDX, IDC_DSNAPGRID, m_d_snapgrid);
	DDX_Check(pDX, IDC_SHOW_ORIGIN, m_d_showorigin);
	DDX_Check(pDX, IDC_SHOWNAMES, m_d_shownames);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_DFREQ, m_IDC_d_freq);
	DDX_Control(pDX, IDC_DGRIDSIZE, m_IDC_d_gridsize);
	DDX_Control(pDX, IDC_DPIXELS, m_IDC_d_pixels);
	DDX_Control(pDX, IDC_DDEPTH, m_IDC_d_depth);
	DDX_Control(pDX, IDC_DPREC, m_IDC_d_prec);
	DDX_Control(pDX, IDC_DMINANGLE, m_IDC_d_minangle);
}


BEGIN_MESSAGE_MAP(CPref, CDialog)
	//{{AFX_MSG_MAP(CPref)
	ON_BN_CLICKED(IDC_MODBTN, OnModifyButton)
	ON_BN_CLICKED(IDC_RESTORE, OnRestoreColors)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPref message handlers

void CPref::OnModifyButton() 
{
	CColorDialog dlg;
	int i;

	UpdateData();
	i=m_d_color.GetCurSel();

    dlg.m_cc.lpCustColors=clist; 
	dlg.m_cc.rgbResult=clist[i];
	dlg.m_cc.Flags=dlg.m_cc.Flags | CC_FULLOPEN | CC_RGBINIT;

	if(dlg.DoModal()==IDOK){
		clist[i]=dlg.GetColor();
	}
	else{
	}

}

BOOL CPref::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	ScanPrefs();

	m_d_action.SetCurSel(s_action);
	m_d_coord.SetCurSel(s_coord);
	m_d_length.SetCurSel(s_length);
	m_d_type.SetCurSel(s_type);
	m_d_solver.SetCurSel(s_solver);
	m_d_color.SetCurSel(0);

	UpdateData(FALSE);

	return TRUE;  
}

void CPref::OnRestoreColors() 
{
	if(AfxMessageBox("Reset all color preferences?",MB_OKCANCEL)==IDOK)
	{
		clist[0]= dSelColor;
		clist[1]= dMeshColor;
		clist[2]= dBlockColor; 
		clist[3]= dLineColor;
		clist[4]= dGridColor;
		clist[5]= dNodeColor;
		clist[6]= dBackColor;
		clist[7]= dNameColor;
	}
}

BOOL CPref::PreTranslateMessage(MSG* pMsg)
{
	 // Pressing ENTER should reroute message to parent
	if( (pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_RETURN) )
	{
		GetParent()->PostMessage(WM_KEYDOWN, VK_RETURN, 0);
		// Message needs no further processing
		return TRUE;
	}

	// Pressing ESC should reroute message to parent
	if( (pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_ESCAPE) )
	{
		GetParent()->PostMessage(WM_KEYDOWN, VK_ESCAPE, 0);
		// Message needs no further processing
		return TRUE;
	}

	// Allow default handler otherwise
	return CDialog::PreTranslateMessage(pMsg);
} 

char* StripKey(char *c);

void CPref::ScanPrefs()
{
	FILE *fp;
	CString fname = ((CFemmApp *)AfxGetApp())->GetExecutablePath() + "femme.cfg";

	fp=fopen(fname,"rt");
	if (fp!=NULL)
	{
		BOOL flag=FALSE;
		char s[1024];
		char q[1024];
		char *v;
		int cr,cg,cb;

		// parse the file
		while (fgets(s,1024,fp)!=NULL)
		{
			sscanf(s,"%s",q);

			if( _strnicmp(q,"<Precision>",11)==0)
			{
			  v=StripKey(s);
			  sscanf(v,"%lf",&m_d_prec);
			  q[0]=NULL;
			}


			if( _strnicmp(q,"<MinAngle>",10)==0)
			{
			  v=StripKey(s);
			  sscanf(v,"%lf",&m_d_minangle);
			  q[0]=NULL;
			}

			if( _strnicmp(q,"<Frequency>",11)==0)
			{
			  v=StripKey(s);
			  sscanf(v,"%lf",&m_d_freq);
			  q[0]=NULL;
			}

			if( _strnicmp(q,"<Depth>",7)==0)
			{
			  v=StripKey(s);
			  sscanf(v,"%lf",&m_d_depth);
			  q[0]=NULL;
			}

			if( _strnicmp(q,"<Coordinates>",13)==0)
			{
			  v=StripKey(s);
			  sscanf(v,"%i",&s_coord);
			  q[0]=NULL;
			}
				
			if( _strnicmp(q,"<LengthUnits>",13)==0)
			{
			  v=StripKey(s);
			  sscanf(v,"%i",&s_length);
			  q[0]=NULL;
			}

			if( _strnicmp(q,"<ProblemType>",13)==0)
			{
			  v=StripKey(s);
			  sscanf(v,"%i",&s_type);
			  q[0]=NULL;
			}
			
			if( _strnicmp(q,"<ACSolver>",8)==0)
			{
			  v=StripKey(s);
			  sscanf(v,"%i",&s_solver);
			  q[0]=NULL;
			}

			if( _strnicmp(q,"<SelColor>",10)==0)
			{
			  v=StripKey(s);
			  sscanf(v,"%i	%i	%i",&cr,&cg,&cb);
			  SelColor=RGB(cr,cg,cb);
			  q[0]=NULL;
			}

			if( _strnicmp(q,"<BkgndColor>",12)==0)
			{
			  v=StripKey(s);
			  sscanf(v,"%i	%i	%i",&cr,&cg,&cb);
			  BackColor=RGB(cr,cg,cb);
			  q[0]=NULL;
			}

			if( _strnicmp(q,"<MeshColor>",10)==0)
			{
			  v=StripKey(s);
			  sscanf(v,"%i	%i	%i",&cr,&cg,&cb);
			  MeshColor=RGB(cr,cg,cb);
			  q[0]=NULL;
			}
			
			if( _strnicmp(q,"<BlockColor>",10)==0)
			{
			  v=StripKey(s);
			  sscanf(v,"%i	%i	%i",&cr,&cg,&cb);
			  BlockColor=RGB(cr,cg,cb);
			  q[0]=NULL;
			}

			if( _strnicmp(q,"<LineColor>",10)==0)
			{
			  v=StripKey(s);
			  sscanf(v,"%i	%i	%i",&cr,&cg,&cb);
			  LineColor=RGB(cr,cg,cb);
			  q[0]=NULL;
			}
	
			if( _strnicmp(q,"<GridColor>",10)==0)
			{
			  v=StripKey(s);
			  sscanf(v,"%i	%i	%i",&cr,&cg,&cb);
			  GridColor=RGB(cr,cg,cb);
			  q[0]=NULL;
			}

			if( _strnicmp(q,"<NodeColor>",10)==0)
			{
			  v=StripKey(s);
			  sscanf(v,"%i	%i	%i",&cr,&cg,&cb);
			  NodeColor=RGB(cr,cg,cb);
			  q[0]=NULL;
			}

			if( _strnicmp(q,"<NameColor>",10)==0)
			{
			  v=StripKey(s);
			  sscanf(v,"%i	%i	%i",&cr,&cg,&cb);
			  NameColor=RGB(cr,cg,cb);
			  q[0]=NULL;
			}

			if( _strnicmp(q,"<EditAction>",12)==0)
			{
			  v=StripKey(s);
			  sscanf(v,"%i",&s_action);
			  q[0]=NULL;
			}
		
			if( _strnicmp(q,"<PixelsPerUnit>",15)==0)
			{
			  v=StripKey(s);
			  sscanf(v,"%lf",&m_d_pixels);
			  q[0]=NULL;
			}
			
			if( _strnicmp(q,"<GridSize>",10)==0)
			{
			  v=StripKey(s);
			  sscanf(v,"%lf",&m_d_gridsize);
			  q[0]=NULL;
			}
			
			if( _strnicmp(q,"<ShowGrid>",10)==0)
			{
			  v=StripKey(s);
			  sscanf(v,"%i",&m_d_showgrid);
			  q[0]=NULL;
			}
		
			if( _strnicmp(q,"<ShowOrigin>",12)==0)
			{
			  v=StripKey(s);
			  sscanf(v,"%i",&m_d_showorigin);
			  q[0]=NULL;
			}
			
			if( _strnicmp(q,"<SnapGrid>",10)==0)
			{
			  v=StripKey(s);
			  sscanf(v,"%i",&m_d_snapgrid);
			  q[0]=NULL;
			}

			if( _strnicmp(q,"<ShowNames>",10)==0)
			{
			  v=StripKey(s);
			  sscanf(v,"%i",&m_d_shownames);
			  q[0]=NULL;
			}
		}
		fclose(fp);
	}
}

void WriteColor(char *cname, COLORREF c,FILE *fp);

void CPref::WritePrefs()
{	
	FILE *fp;
	CString fname;
	
	UpdateData();
	s_action=m_d_action.GetCurSel();
	s_coord=m_d_coord.GetCurSel();
	s_length=m_d_length.GetCurSel();
	s_type=m_d_type.GetCurSel();
	s_solver=m_d_solver.GetCurSel();
	fname=((CFemmApp *)AfxGetApp())->GetExecutablePath()+"femme.cfg";

	fp=fopen(fname,"wt");
	if (fp!=NULL)
	{
		WriteColor("SelColor",SelColor,fp);
		WriteColor("BkgndColor",BackColor,fp);
		WriteColor("MeshColor",MeshColor,fp);
		WriteColor("BlockColor",BlockColor,fp);
		WriteColor("LineColor",LineColor,fp);
		WriteColor("GridColor",GridColor,fp);
		WriteColor("NodeColor",NodeColor,fp);
		WriteColor("NameColor",NameColor,fp);
		fprintf(fp,"<EditAction> = %i\n",s_action);
		fprintf(fp,"<PixelsPerUnit> = %g\n",m_d_pixels);
		fprintf(fp,"<GridSize> = %g\n",m_d_gridsize);
		fprintf(fp,"<ShowGrid> = %i\n",m_d_showgrid);
		fprintf(fp,"<SnapGrid> = %i\n",m_d_snapgrid);
		fprintf(fp,"<ShowNames> = %i\n",m_d_shownames);
		fprintf(fp,"<ShowOrigin> = %i\n",m_d_showorigin);
		fprintf(fp,"<ProblemType> = %i\n",s_type);
		fprintf(fp,"<ACSolver> = %i\n",s_solver);
		fprintf(fp,"<LengthUnits> = %i\n",s_length);
		fprintf(fp,"<Frequency> = %g\n",m_d_freq);
		fprintf(fp,"<Precision>  = %g\n",m_d_prec);
		fprintf(fp,"<MinAngle>  = %g\n",m_d_minangle);
		fprintf(fp,"<Depth> = %g\n",m_d_depth);
		fprintf(fp,"<Coordinates> = %i\n",s_coord);
	
		fclose(fp);
	}

}

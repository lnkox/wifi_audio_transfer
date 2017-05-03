VERSION 5.00
Object = "{248DD890-BB45-11CF-9ABC-0080C7E7B78D}#1.0#0"; "MSWINSCK.OCX"
Begin VB.Form Form1 
   Caption         =   "Form1"
   ClientHeight    =   9210
   ClientLeft      =   120
   ClientTop       =   450
   ClientWidth     =   10260
   LinkTopic       =   "Form1"
   ScaleHeight     =   9210
   ScaleWidth      =   10260
   StartUpPosition =   3  'Windows Default
   Begin VB.CommandButton Command2 
      Caption         =   "Command2"
      Height          =   1455
      Left            =   3360
      TabIndex        =   1
      Top             =   1080
      Width           =   3255
   End
   Begin VB.CommandButton Command1 
      Caption         =   "Command1"
      Height          =   255
      Left            =   1560
      TabIndex        =   0
      Top             =   360
      Width           =   1455
   End
   Begin MSWinsockLib.Winsock Winsock1 
      Left            =   1320
      Top             =   1080
      _ExtentX        =   741
      _ExtentY        =   741
      _Version        =   393216
      Protocol        =   1
      RemoteHost      =   "255.255.255.255"
      RemotePort      =   2214
      LocalPort       =   2211
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Dim nbyte As Long

Private Sub Form_Load()
Open "c:\1.wav" For Binary As #1
nbyte = 1
Winsock1.Bind


End Sub

Private Sub Form_Unload(Cancel As Integer)
Close #1
End Sub

Private Sub Winsock1_DataArrival(ByVal bytesTotal As Long)
Dim data As String
Winsock1.GetData data

Winsock1.SendData data
 Me.Caption = nbyte

'Put #1, nbyte, data
nbyte = nbyte + bytesTotal

End Sub


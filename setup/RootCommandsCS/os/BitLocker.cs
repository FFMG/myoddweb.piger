using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Management.Automation;

using ActionMonitor.Interfaces;

namespace ActionMonitor.Plugin
{
  public class BitLocker
  {
    /// <summary>
    /// Enum for the posible lock status
    /// </summary>
    private enum LockStatus
    {
      NotBitlocker,
      Locked,
      Unlocked
    }

    /// <summary>
    /// What the user selected
    /// </summary>
    private class UserSelection 
    { 
      public DriveInfo _drive;
      public string _password;
      public bool _lock;
      public UserSelection( DriveInfo drive, string password, bool lockDrive )
      {
        _drive = drive;
        _password = password;
        _lock = lockDrive;
      }
    }

    /// <summary>
    /// Information about the drive
    /// </summary>
    private class ComboboxItem 
    { 
      public DriveInfo _drive;
      public LockStatus _status;
      public ComboboxItem( DriveInfo drive, LockStatus status )
      {
        _drive = drive;
        _status = status;
      }
      public override string ToString() 
      { 
        var statusText = "Locked";
        switch (_status)
        {
        case LockStatus.NotBitlocker:
          statusText = "Not bitlocker";
          break;

        case LockStatus.Locked:
          statusText = "Locked";
          break;

        case LockStatus.Unlocked :
          statusText = "Unlocked";
          break;

        default:
          statusText = "Unknown";
          break;
        }
        return _drive.Name + " ["+(statusText)+"]"; 
      } 
    }

    /// <summary>
    /// The action monitor.
    /// </summary>
    private IActionMonitor _am;

    /// <summary>
    /// Constructor
    /// </summary>
    public BitLocker(IActionMonitor am)
    {
      _am = am;
    }

    /// <summary>
    /// Get the current lock status of a given drive.
    /// </summary>
    /// <returns>The status of the drive</returns>
    private LockStatus GetLockStatus( DriveInfo drive )
    {
      try
      {
        // get the drive letter
        var d = drive.Name;
        d = d.Trim('\\');

        // get the status of the drive
        // if it throws then it is not a bitlocked drive at all.
        var islocked = RunPowerShellCommand( "(manage-bde -status "+d+"| Select-String -Pattern 'Lock Status').ToString().Split(\":\")[1].trim().ToLower() -eq \"locked\"" , null);
        if( islocked == "True" )
        {
          // if it is locked then it must be a bitlocker drive.
          return LockStatus.Locked;
        } 

        // if it is not locked ... then maybe it is not protected at all?
        var isprotected = RunPowerShellCommand( "(manage-bde -status "+d+"| Select-String -Pattern 'Protection Status').ToString().Split(\":\")[1].trim().ToLower() -eq \"protection on\"" , null);

        // if it is protected, then it is unlocked
        // otherwise it is not really a bitlock drive.
        return isprotected == "True" ? LockStatus.Unlocked : LockStatus.NotBitlocker;
      }
      catch
      {
        return LockStatus.NotBitlocker;
      }
    }

    /// <summary>
    /// Displays a dialog with a prompt and textbox where the user can enter information
    /// </summary>
    /// <returns>Dialog result</returns>
    private UserSelection Show( )
    {
      const string title = "Password:";
      const string promptText = "Enter password:";

      using( var form = new Form() )
      {
        var label = new Label();
        var textBox = new TextBox();
        var buttonOk = new Button();
        var buttonCancel = new Button();
        var comboBoxDrive = new ComboBox();
        var checkBoxLock = new CheckBox();
        form.SuspendLayout();
        // 
        // label
        // 
        label.AutoSize = true;
        label.Location = new Point(9, 20);
        label.Name = "label";
        label.Size = new Size(57, 13);
        label.TabIndex = 0;
        label.Text = promptText;
        // 
        // textBox
        // 
        textBox.Anchor = ((AnchorStyles)(((AnchorStyles.Top | AnchorStyles.Left) | AnchorStyles.Right)));
        textBox.Location = new Point(12, 36);
        textBox.Name = "textBox";
        textBox.PasswordChar = '*';
        textBox.Size = new Size(347, 20);
        textBox.TabIndex = 1;
        textBox.Text = "";
        // 
        // buttonOk
        // 
        buttonOk.Anchor = ((AnchorStyles)((AnchorStyles.Bottom | AnchorStyles.Right)));
        buttonOk.DialogResult = DialogResult.OK;
        buttonOk.Location = new Point(203, 62);
        buttonOk.Name = "buttonOk";
        buttonOk.Size = new Size(75, 23);
        buttonOk.TabIndex = 2;
        buttonOk.Text = "OK";
        // 
        // buttonCancel
        // 
        buttonCancel.Anchor = ((AnchorStyles)((AnchorStyles.Bottom | AnchorStyles.Right)));
        buttonCancel.DialogResult = DialogResult.Cancel;
        buttonCancel.Location = new Point(284, 62);
        buttonCancel.Name = "buttonCancel";
        buttonCancel.Size = new Size(75, 23);
        buttonCancel.TabIndex = 3;
        buttonCancel.Text = "Cancel";
        // 
        // comboBoxDrive
        // 
        comboBoxDrive.FormattingEnabled = true;
        comboBoxDrive.Location = new Point(12, 64);
        comboBoxDrive.Name = "comboBoxDrive";
        comboBoxDrive.Size = new Size(121, 21);
        comboBoxDrive.TabIndex = 4;
        // 
        // checkBox1
        // 
        checkBoxLock.AutoSize = true;
        checkBoxLock.Location = new Point(13, 92);
        checkBoxLock.Name = "checkBoxLock";
        checkBoxLock.Size = new Size(80, 17);
        checkBoxLock.TabIndex = 5;
        checkBoxLock.Text = "Lock drive.";
        checkBoxLock.UseVisualStyleBackColor = true;

        // get allt the drive
        // and select the first one in the list.
        var allDrives = DriveInfo.GetDrives();
        var posibleIndex = 0;
        var index = 0;
        foreach (var d in allDrives)
        {
          // look for the first bitlocker
          var status = GetLockStatus(d);
          switch (status)
          {
          case LockStatus.NotBitlocker:
            break;

          case LockStatus.Locked:
          case LockStatus.Unlocked :
            if( posibleIndex == 0 )
            {
              posibleIndex = index;
            }
            break;

          default:
            break;
          }
          comboBoxDrive.Items.Add( new ComboboxItem( d, status ) );
          index++;
        }

        // set the index.
        comboBoxDrive.SelectedIndex= posibleIndex;

        // 
        // Form1
        // 
        form.AcceptButton = buttonOk;
        form.CancelButton = buttonCancel;
        form.ClientSize = new Size(371, 115);
        form.Controls.Add(comboBoxDrive);
        form.Controls.Add(label);
        form.Controls.Add(textBox);
        form.Controls.Add(buttonOk);
        form.Controls.Add(buttonCancel);
        form.Controls.Add(checkBoxLock);
        form.FormBorderStyle = FormBorderStyle.FixedDialog;
        form.MaximizeBox = false;
        form.MinimizeBox = false;
        form.Name = "Form1";
        form.StartPosition = FormStartPosition.CenterScreen;
        form.Text = title;
        form.ResumeLayout(false);
        form.PerformLayout();

        var dialogResult = form.ShowDialog();
        if( dialogResult != DialogResult.OK )
        {
          return null;
        }

        var selectedCar = (ComboboxItem)comboBoxDrive.SelectedItem;
        return new UserSelection( selectedCar._drive, textBox.Text, checkBoxLock.Checked );
      }
    }

    /// <summary>
    /// Call a powershell command.
    /// </summary>
    private string RunPowerShellCommand(string command, Dictionary<string, object> parameters)
    {
      using (PowerShell powerShellInstance = PowerShell.Create())
      {
        // Set up the running of the script
        powerShellInstance.AddScript(command);

        // Add the parameters
        if (parameters != null)
        {
          foreach (var parameter in parameters)
          {
            powerShellInstance.AddParameter(parameter.Key, parameter.Value);
          }
        }

        // Run the command
        Collection<PSObject> psOutput = powerShellInstance.Invoke();
        var stringBuilder = new StringBuilder();
        if (powerShellInstance.Streams.Error.Count > 0)
        {
          foreach (var errorMessage in powerShellInstance.Streams.Error)
          {
            if (errorMessage != null)
            {
              throw new InvalidOperationException(errorMessage.ToString());
            }
          }
        }

        foreach (var outputLine in psOutput)
        {
          if (outputLine != null)
          {
            stringBuilder.Append(outputLine);
          }
        }

        return stringBuilder.ToString();
      }
    }

    /// <summary>
    /// Called to launche the plugin.
    /// </summary>
    public void Go()
    {
      Console.WriteLine("Action monitor Bitlocker");

      var userSelection = Show( );
      if( userSelection == null )
      {
        _am.Say( "Cancelled.", 100, 5 );
        return;
      }

      try
      {
        if( userSelection._lock )
        {
          string unlockedDrive = RunPowerShellCommand( "Lock-BitLocker -MountPoint \""+userSelection._drive.Name+"\" -ForceDismount", null);

          _am.Say( "Drive '"+userSelection._drive.Name+"'('"+unlockedDrive+"') was Locked.", 100, 5 );
        }
        else
        {
          string unlockedDrive = RunPowerShellCommand(
            "$SecureString = ConvertTo-SecureString \"" + userSelection._password + "\" -AsPlainText -Force\n" +
            "Unlock-BitLocker -MountPoint \""+userSelection._drive.Name+"\" -Password $SecureString", null);

            _am.Say( "Drive '"+userSelection._drive.Name+"'('"+unlockedDrive+"') was unlocked.", 100, 5 );
        }
      }
      catch (System.Exception e )
      {
          _am.Log( 2, e.ToString() );
          _am.Say( "There was an error trying to unlock drive '"+userSelection._drive.Name+"' ... is the password valid? Is it a bitlock drive?", 300, 5 );
      }
    }
  }
}

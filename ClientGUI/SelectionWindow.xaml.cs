//////////////////////////////////////////////////////////////////////
// Yuexin Yu                                                        //
// CSE 687 Object Oriented Design C++                               //
// Syracuse University                                              //
// Design Project: Test Harness                                     //
// yyu100@syr.edu                                                   //
//                                                                  //
// This package defines one class SelectionWindow that provides a   //
// ListBox for displaying files selected in the main window.        //
//////////////////////////////////////////////////////////////////////

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;
using MsgPassingCommunication;
using System.Threading;

namespace GUI
{
    
    public partial class SelectionWindow : Window
    {
        MainWindow mWin = null;

        public SelectionWindow()
        {
            InitializeComponent();
        }


        // Called by MainWindow to give this window her ref
        public void setMainWindow(MainWindow mwin)
        {
            mWin = mwin;
        }

        // Called by MainWindow to add a selected file
        public void Add(string file)
        {
            Selections.Items.Add(file);
            mWin.swin.Topmost = true;
        }

        // Unselected files and set mWin.swin to null
        private void Window_Unloaded(object sender, RoutedEventArgs e)
        {
            mWin.unselectFiles();
            mWin.swin = null;
        }

        // May need this when SelectionWindow is triggered
        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            this.Topmost = true;
        }

        // Save selected files to current test request
        private void Done_Click(object sender, RoutedEventArgs e)
        {
            int count = mWin.selectedFiles.Count() - 1;

            string key = "sendingFile_" + count;
            string item_name = System.IO.Path.GetFileName(mWin.selectedFiles[count]);

            mWin.msg.add(key, item_name);

            new Thread(() =>
            {
                this.Dispatcher.Invoke(new Action(() =>
                {
                   mWin.show_dlls.Items.Add(mWin.selectedFiles[count]);
                }));
            }).Start();

            mWin.swin.Close();
        }

        // Clear current selections and close selection window
        private void Clear_Click(object sender, RoutedEventArgs e)
        {
            mWin.selectedFiles.Clear();
            mWin.show_dlls.Items.Clear();
            Selections.Items.Clear();
            mWin.swin.Close();
        }
    }
}

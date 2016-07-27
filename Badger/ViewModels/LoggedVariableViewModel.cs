﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml;
using System.Dynamic;
using System.Windows;
using Caliburn.Micro;
using System.IO;

namespace AppXML.ViewModels
{
    public class LoggedVariableViewModel:PropertyChangedBase
    {
        private bool m_bIsSelected = false;
        public bool bIsSelected {
            get { return m_bIsSelected; }
            set {m_bIsSelected= value;
                NotifyOfPropertyChange(()=>bIsSelected);
                m_parent.updateVariableListHeader();
            }}

        private string m_name;
        public string name { get { return m_name; }
            set { m_name = value; NotifyOfPropertyChange(() => name); }
        }

        private PlotEditorWindowViewModel m_parent = null;

        public LoggedVariableViewModel(string name,PlotEditorWindowViewModel parent)
        {
            m_parent = parent;
            m_name = name;
        }
    }
}
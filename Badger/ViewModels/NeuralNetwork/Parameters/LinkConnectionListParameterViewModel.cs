/*
	SimionZoo: A framework for online model-free Reinforcement Learning on continuous
	control problems

	Copyright (c) 2016 SimionSoft. https://github.com/simionsoft

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.
*/

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Badger.Data.NeuralNetwork.Parameters;
using System.Collections.ObjectModel;
using GongSolutions.Wpf.DragDrop;
using Badger.ViewModels.NeuralNetwork.Links;

namespace Badger.ViewModels.NeuralNetwork.Parameters
{
    class LinkConnectionListParameterViewModel : ParameterBaseViewModel, IDropTarget
    {
        public LinkConnectionListParameter LinkConnectionListParameterData { get; }
        public LinkConnectionListParameterViewModel(LinkConnectionListParameter linkConnectionListParameter, LinkBaseViewModel parent) : base(linkConnectionListParameter, parent)
        {
            LinkConnectionListParameterData = linkConnectionListParameter;

            Value = new ObservableCollection<LinkConnectionViewModel>();
            refreshValue();
        }

        public ObservableCollection<LinkConnectionViewModel> Value { get; set; }

        private void refreshValue()
        {
            Value.Clear();
            foreach (var connection in LinkConnectionListParameterData.Value)
                Value.Add(new LinkConnectionViewModel(connection));
        }

        public void DragOver(IDropInfo dropInfo)
        {
            if (dropInfo.Data is LinkBaseViewModel)
            {
                dropInfo.Effects = System.Windows.DragDropEffects.Copy;
                dropInfo.NotHandled = false;
                dropInfo.DropTargetAdorner = DropTargetAdorners.Insert;
            }
        }

        public void Drop(IDropInfo dropInfo)
        {
            var item = dropInfo.Data as LinkBaseViewModel;

            if (item == null)
                return;

            LinkConnectionListParameterData.Value.Add(new LinkConnection(item.LinkBaseData));
            refreshValue();
        }
    }
}

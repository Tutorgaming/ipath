/* iPath: A C++ Library of Intelligent Global Path Planners for Mobile Robots with ROS Integration. 

 * Website: http://www.iroboapp.org/index.php?title=IPath
 * Contact: 
 *
 * Copyright (c) 2014
 * Owners: Al-Imam University/King AbdulAziz Center for Science and Technology (KACST)/Prince Sultan University
 * All rights reserved.
 *
 * License Type: GNU GPL
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "OccupancyGridMap.h"
#include<limits.h>
using namespace std;

 //constructors
 OccupancyGridMap ::OccupancyGridMap(int w, int h, float res) : Map( w, h, res){
 }
 
 OccupancyGridMap :: OccupancyGridMap (int w, int h, float res, int ** mapMatrix, int obsSize, float obsRatio) : Map( w, h, res, mapMatrix, obsSize, obsRatio){ 
 }
 
 OccupancyGridMap::OccupancyGridMap (OccupancyGridMap * map): Map (map){
 }
 
 //the cell(i,j) will be an obstacle
 void OccupancyGridMap::setObstacle (int i, int j){
   getMapLayout()[i][j]=OBSTACLE;
 }
 
  //the cell(i,j) will be free
 void OccupancyGridMap::setFree (int i, int j){
    getMapLayout()[i][j]=FREE;
 }
 
  //the cell(i,j) will be unknown
 void OccupancyGridMap::setUnknown (int i, int j){
    getMapLayout()[i][j]=UNKNOWN;
 } 
 
 //give the cell(i,j) the value of occupancy
 void OccupancyGridMap::setCell (int i, int j, int occupancy){
   getMapLayout()[i][j]=occupancy;
 }
 
 float  OccupancyGridMap::getMoveCost(int i1, int j1, int i2, int j2){
   float moveCost=INFINIT_COST;//start cost with maximum value. Change it to real cost of cells are connected
   //if cell2(i2,j2) exists in the diagonal of cell1(i1,j1)
   if((j2==j1+1 && i2==i1+1)||(i2==i1-1 && j2==j1+1) ||(i2==i1-1 && j2==j1-1)||(j2==j1-1 && i2==i1+1)){
     moveCost = DIAGONAL_MOVE_COST;
   }
    //if cell 2(i2,j2) exists in the horizontal or vertical line with cell1(i1,j1)
   else{
     if ((j2==j1 && i2==i1-1)||(i2==i1 && j2==j1-1)||(i2==i1+1 && j2==j1) ||(i1==i2 && j2==j1+1)){
       moveCost = MOVE_COST;
     }
   }
   return moveCost;
 } 
 
 
  float  OccupancyGridMap::getMoveCost(int CellID1, int CellID2){
   int i1=0,i2=0,j1=0,j2=0;
    
   i1=getCellRowID(CellID1);
   j1=getCellColID(CellID1);
   i2=getCellRowID(CellID2);
   j2=getCellColID(CellID2);
    
    return getMoveCost(i1, j1, i2, j2);
 } 
 
 
 
 //verify if the cell(i,j) is an obstacle
 bool  OccupancyGridMap::isObstacle(int i, int j){
   return( getMapLayout()[i][j]==OBSTACLE);
 }
 //verify if the cell(i,j) is free
 bool  OccupancyGridMap::isFree(int i, int j){
 return (getMapLayout()[i][j]==FREE);
 } 
 //verify if the cell(i,j) is unknown
 bool  OccupancyGridMap::isUnknown(int i, int j){
   return (getMapLayout()[i][j]==UNKNOWN);
 }
  //verify if the cell(i,j) is an obstacle
 bool  OccupancyGridMap::isObstacle(int CellID){
   return(getMapLayout()[getCellRowID(CellID)][getCellColID(CellID)]==OBSTACLE);
 }
  //verify if the cell(i,j) is free
 bool  OccupancyGridMap::isFree(int CellID){
 return (getMapLayout()[getCellRowID(CellID)][getCellColID(CellID)]==FREE);
 } 
  //verify if the cell(i,j) is unknown
 bool  OccupancyGridMap::isUnknown(int CellID){
   return (getMapLayout()[getCellRowID(CellID)][getCellColID(CellID)]==UNKNOWN);
 }
 
 
 // write the MapLayout matrix in a file
void OccupancyGridMap::exportMapLayout(const char* file_name, int** mapMatrix)
{
   ofstream file (file_name, ios::trunc);
   /*find the number of obstacle cells*/
   double sumObs=0;
     for(int i=0;i<getHeight();i++)
    {
      for(int j=0;j<getWidth();j++)
      {
	if (getMapLayout()[i][j]==OBSTACLE)
	  sumObs++;
      }
      
      }
    /*write the PGM file*/
    file<<"P2";
    file<<endl;
    file<<"#Generated by exportMapLayout in OccupancyGridMap.h. Obstacle Ratio=";
    file<<sumObs/(getWidth()*getHeight());
    file<<endl;
    file<<getWidth();
    file<<" ";
    file<<getHeight();
    file<<endl;
    file<<"100";
    file<<endl;
    for(int i=0;i<getHeight();i++)
    {
      for(int j=0;j<getWidth();j++)
      {
	file<<mapMatrix[i][j];
	if(j<getWidth()-1)
	  file<<" ";
      }
      if(i<getHeight()-1)
	file<<endl;
      }
}
 
 

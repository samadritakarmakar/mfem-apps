/*Cloud FEA lets people access research based FEA software via a browser based UI
    Copyright (C) 2020  Samadrita Karmakar (samadritakarmakar@gmial.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.*/
const express = require('express');
var multer  = require('multer')
var meshDir = __dirname + '/meshFile';
var upload = multer({ dest: meshDir+'/' })
const fs = require('fs');
const bodyParser = require('body-parser');
const path = require('path');
const create_array = require('create_array');
const {exec} = require('child_process');

port = 3000;

var publicDir = __dirname+'/public';
var viewDir = __dirname+'/public/profile';
var meshFileName = 'meshFile';
var MeshPath ={path: meshDir+'/'+meshFileName + '.msh'};
var problemConfig = __dirname + '/problemConfig';
var ProblemConfigPath = {path: problemConfig+'/problemConfig.JSON'};
var outputFileName = 'PVLinearElastic.zip';
//var executablePath = __dirname + '/.././LinearElastic -jc '
var executablePath = '/home/sam/Programs/mfem/mfem-apps/build/Debug'+ '/LinearElastic -jc ';
var app = express();
app.set('view engine', 'ejs');
app.set('views', viewDir);
app.set('views', publicDir);
app.use('/js', express.static(publicDir+'/js'));
app.use('/css',express.static(publicDir+'/css'));
app.use('/img', express.static(publicDir+'/img'));

app.listen(port);


app.get('/', (req, res) => {
  RequestRespond ={request: req, respond: res, dir: publicDir, filename: '/index', fs: fs};
  request_n_respond(RequestRespond);
});
app.get('/SolidMechanics/LinearElasticity', (req, res) => {
  RequestRespond ={request: req, respond: res, dir: publicDir, filename: req.url, fs: fs};
  request_n_respond(RequestRespond);
});
app.get('/SolidMechanics/*', (req, res) => {
  console.log(`Requested url is ${req.url}`);
  res.sendFile(__dirname+'/public'+req.url);
});

/*app.get('/contact', (req, res) => {
app.set('views', publicDir);
RequestRespond ={request: req, respond: res, dir: publicDir, filename: req.url, fs: fs};
TemplateRender(RequestRespond);
});
app.get('/profile/:name', (req, res) => {
  app.set('views', viewDir);
  RequestRespond ={request: req, respond: res, dir: viewDir, filename: req.url, fs: fs};
  TemplateRender(RequestRespond);
});


app.post('/contact', urlEncodedParser, (req, res) => {
  app.set('views', publicDir);
  console.log(req.body);
  RequestRespond ={request: req, respond: res, dir: publicDir, filename: 'contactsuccess', fs: fs};
  TemplateRender(RequestRespond);
});*/



// SET STORAGE
var storage = multer.diskStorage({
    destination: (req, file, cb) => {
      cb(null, meshDir);
    },
    filename: (req, file, cb) => {
      cb(null, file.fieldname+path.extname(file.originalname));
      if(!containsMsh_extn(MeshPath.path))
      {
        MeshPath.path = MeshPath.path+'/'+file.fieldname+path.extname(file.originalname);
      }
    }
});

var upload = multer({storage: storage});
app.post('/SolidMechanics/LinearElasticProblemDef', upload.single('meshFile'), (req, res)=>
{
  RequestRespond ={request: req, respond: res, dir: 'SolidMechanics', filename: 'LinearElasticityProblemDef', fs: fs};
  app.set('views', publicDir+'/SolidMechanics');
  TemplateRender(RequestRespond);
});

// create application/x-www-form-urlencoded parser
var urlEncodedParser = bodyParser.urlencoded({ extended: false })
app.post('/SolidMechanics/LinearElasticity/problemsubmit',  urlEncodedParser, (req, res) => {
  app.set('views', publicDir);
  console.log(req.body);
  var problemDef =
  {
    mesh: MeshPath.path,
    order: parseInt(req.body.order),
    maxMeshRefinement: parseInt(req.body.maxMeshRefinement),
    dirichletArributes: saveObjectsToArray1D(req.body.dirichletArributes),
    dirichletValues: saveObjectsToArray2D(req.body.xdiric, req.body.ydiric, req.body.zdiric),
    activeDirichletValues: saveObjectsToArray2D(req.body.xActivateDirich, req.body.yActivateDirich, req.body.zActivateDirich),
    neumannArributes: saveObjectsToArray1D(req.body.neumannArributes),
    neumannValues: saveObjectsToArray2D(req.body.xneum, req.body.yneum, req.body.zneum),
    materialAttirbutes: saveObjectsToArray1D(req.body.materialAttirbutes),
    elasticModulus: saveObjectsToArray1D(req.body.elasticModulus),
    nu: saveObjectsToArray1D(req.body.nu),
    visualization:false
  }
  console.log(`problemDef:\n`);
  console.log(problemDef);
  var problemDefString = JSON.stringify(problemDef);
  WriteThisToFile(problemDefString, ProblemConfigPath.path);
  var exe = '/usr/bin/zip -r '+outputFileName+' PVLinearElastic';
  var Pass ={exe: exe, res: res, outputFileName: outputFileName};
  executeThis(executablePath+ProblemConfigPath.path, Pass, null, (Pass)=>{
    executeThis(Pass.exe, Pass, null, (Pass)=>{
      console.log(`Sending ${Pass.outputFileName} to client.`);
      Pass.res.download(Pass.outputFileName);
    });
  });
});

var saveObjectsToArray1D = function(x)
{
  if (Array.isArray(x))
  {
    var length = x.length;
    var array1D = create_array.create1D_Array(length);
    for (var i=0; i<length; i++)
    {
      array1D[i] = stringToExponent(x[i]);
    }
  }
  else
  {
    var array1D = create_array.create1D_Array(1);
    array1D[0] = stringToExponent(x);
  }
  return array1D;
}

var saveObjectsToArray2D = function(x,y,z)
{
  if (Array.isArray(x))
  {
    var length = x.length;
    var array2D = create_array.create2D_Array(length,3);
    for(var i=0; i<length; i++)
    {
      array2D[i][0]=stringToExponent(x[i]);
      array2D[i][1]=stringToExponent(y[i]);
      array2D[i][2]=stringToExponent(z[i]);

    }
  }
  else
  {
    var array2D = create_array.create2D_Array(1,3);
    array2D[0][0]=stringToExponent(x);
    array2D[0][1]=stringToExponent(y);
    array2D[0][2]=stringToExponent(z);
  }
  return array2D;
}

var stringToExponent = function(numberString)
{
  pos = numberString.search('e');
  if(pos>=0)
  {
    var base = parseFloat(numberString.slice(0, pos));
    var exponent = parseFloat(numberString.slice(pos+1));
    return base*Math.pow(10, exponent);
  }
  else
  {
    return parseFloat(numberString);
  }
}

var containsMsh_extn = function(pathname)
{
  if(pathname.search('.msh')>=0)
  {
    return true;
  }
  return false;
}


var request_n_respond = function(RequestRespond, extname = '.html')
{
  var path = RequestRespond.dir + RequestRespond.filename;
  console.log(`Requested Url is:\n ${RequestRespond.request.url}`);
  console.log(`Query is: `)
  console.log(RequestRespond.request.query);
  /*if(path.search('\\.')!=-1)
  {
    path= path+'.html';
  }*/
  path =addExtn_b4_query(path, extname)
  console.log(`Respond path is:\n ${path}`);

  if (RequestRespond.fs.existsSync(path))
  {
    RequestRespond.respond.sendFile(path);
  }
  else
  {
    RequestRespond.respond.sendFile(__dirname + '/public/NotFound.html');
  }
}

var TemplateRender = function(RequestRespond)
{
  console.log(`Query is: `);
  console.log(RequestRespond.request.query);
  var ejsFile = RequestRespond.filename;
  if(ejsFile.charAt(0)==='\/')
  {
    ejsFile = ejsFile.slice(1);
  }
  ejsFile = addExtn_b4_query(ejsFile, '.ejs');
  console.log(`ejs file is ${ejsFile}`);
  RequestRespond.respond.render(ejsFile, {person: RequestRespond.request.params.name, data: RequestRespond.request.body, query: RequestRespond.request.query}/*,
  (err, str)=>{
    var profileFile = RequestRespond.dir+RequestRespond.filename;
    profileFile = addExtn_b4_query(profileFile, '.html')
    WriteThisToFile(str, profileFile, err);
  RequestRespond.respond.sendFile(profileFile);
}*/);
}

/*var profileRender = function(RequestRespond)
{
  console.log(`Query is: \n ${RequestRespond.request.query}`);
  var data = {age: 29, job: 'ninja', hobbies: ['eating', 'fighting', 'fishing']}
  RequestRespond.respond.render('profile', {person: RequestRespond.request.params.name, data: data},
  (err, str)=>{
    var profileFile = RequestRespond.dir+'/profile.html';
    WriteThisToFile(str, profileFile, err);
  RequestRespond.respond.sendFile(profileFile);
});
}*/

var WriteThisToFile=function(input_str, fileName, error)
{
  console.log(`Trying to write to file ${fileName}`);
  if(error)
  {
    console.log(`We faced this error: ${error}`);
  }
  else
  {
    fs.writeFileSync(fileName, input_str);//, (err)=>{console.log(`Could not write. Faced error:\n ${err}`); });
  }
}

var executeThis = function(pathCommandArgument, arg1, arg2, callback=function(arg1,arg2){})
{
  exec(path.resolve(pathCommandArgument), (error, stdout, stderr)=>
  {
    if(error)
    {
      console.log(`We faced error: \n ${error}`);
    }
    else if(stdout)
    {
      console.log(`Output is: \n\n ${stdout.toString('utf8')}`);
      callback(arg1, arg2);
    }
  });
}

var addExtn_b4_query = function(fileName, extn)
{
  if(!fileName.search('\\.')!=-1)
  {
    var queryPos = fileName.search('\\?');
    if(queryPos===-1)
    {
      fileName= fileName+extn;
    }
    else
    {
      fileName = fileName.slice(0, queryPos)+extn;
    }
  }
  return fileName;
}

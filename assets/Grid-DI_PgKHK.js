import{r as w,g as P,c as R,s as T,d as D,e as I,j as W,f as K,h as $,i as S,k as O}from"./index-Cr12mYfS.js";import{u as L}from"./Stack-B-lg33CO.js";const C=w.createContext();function U(e){return P("MuiGrid",e)}const Z=[0,1,2,3,4,5,6,7,8,9,10],A=["column-reverse","column","row-reverse","row"],F=["nowrap","wrap-reverse","wrap"],m=["auto",!0,1,2,3,4,5,6,7,8,9,10,11,12],g=R("MuiGrid",["root","container","item","zeroMinWidth",...Z.map(e=>`spacing-xs-${e}`),...A.map(e=>`direction-xs-${e}`),...F.map(e=>`wrap-xs-${e}`),...m.map(e=>`grid-xs-${e}`),...m.map(e=>`grid-sm-${e}`),...m.map(e=>`grid-md-${e}`),...m.map(e=>`grid-lg-${e}`),...m.map(e=>`grid-xl-${e}`)]);function _({theme:e,ownerState:n}){let i;return e.breakpoints.keys.reduce((t,r)=>{let s={};if(n[r]&&(i=n[r]),!i)return t;if(i===!0)s={flexBasis:0,flexGrow:1,maxWidth:"100%"};else if(i==="auto")s={flexBasis:"auto",flexGrow:0,flexShrink:0,maxWidth:"none",width:"auto"};else{const a=$({values:n.columns,breakpoints:e.breakpoints.values}),c=typeof a=="object"?a[r]:a;if(c==null)return t;const u=`${Math.round(i/c*1e8)/1e6}%`;let o={};if(n.container&&n.item&&n.columnSpacing!==0){const l=e.spacing(n.columnSpacing);if(l!=="0px"){const p=`calc(${u} + ${l})`;o={flexBasis:p,maxWidth:p}}}s={flexBasis:u,flexGrow:0,maxWidth:u,...o}}return e.breakpoints.values[r]===0?Object.assign(t,s):t[e.breakpoints.up(r)]=s,t},{})}function q({theme:e,ownerState:n}){const i=$({values:n.direction,breakpoints:e.breakpoints.values});return S({theme:e},i,t=>{const r={flexDirection:t};return t.startsWith("column")&&(r[`& > .${g.item}`]={maxWidth:"none"}),r})}function N({breakpoints:e,values:n}){let i="";Object.keys(n).forEach(r=>{i===""&&n[r]!==0&&(i=r)});const t=Object.keys(e).sort((r,s)=>e[r]-e[s]);return t.slice(0,t.indexOf(i))}function H({theme:e,ownerState:n}){const{container:i,rowSpacing:t}=n;let r={};if(i&&t!==0){const s=$({values:t,breakpoints:e.breakpoints.values});let a;typeof s=="object"&&(a=N({breakpoints:e.breakpoints.values,values:s})),r=S({theme:e},s,(c,u)=>{const o=e.spacing(c);return o!=="0px"?{marginTop:`calc(-1 * ${o})`,[`& > .${g.item}`]:{paddingTop:o}}:a?.includes(u)?{}:{marginTop:0,[`& > .${g.item}`]:{paddingTop:0}}})}return r}function J({theme:e,ownerState:n}){const{container:i,columnSpacing:t}=n;let r={};if(i&&t!==0){const s=$({values:t,breakpoints:e.breakpoints.values});let a;typeof s=="object"&&(a=N({breakpoints:e.breakpoints.values,values:s})),r=S({theme:e},s,(c,u)=>{const o=e.spacing(c);if(o!=="0px"){const l=`calc(-1 * ${o})`;return{width:`calc(100% + ${o})`,marginLeft:l,[`& > .${g.item}`]:{paddingLeft:o}}}return a?.includes(u)?{}:{width:"100%",marginLeft:0,[`& > .${g.item}`]:{paddingLeft:0}}})}return r}function Q(e,n,i={}){if(!e||e<=0)return[];if(typeof e=="string"&&!Number.isNaN(Number(e))||typeof e=="number")return[i[`spacing-xs-${String(e)}`]];const t=[];return n.forEach(r=>{const s=e[r];Number(s)>0&&t.push(i[`spacing-${r}-${String(s)}`])}),t}const X=T("div",{name:"MuiGrid",slot:"Root",overridesResolver:(e,n)=>{const{ownerState:i}=e,{container:t,direction:r,item:s,spacing:a,wrap:c,zeroMinWidth:u,breakpoints:o}=i;let l=[];t&&(l=Q(a,o,n));const p=[];return o.forEach(f=>{const d=i[f];d&&p.push(n[`grid-${f}-${String(d)}`])}),[n.root,t&&n.container,s&&n.item,u&&n.zeroMinWidth,...l,r!=="row"&&n[`direction-xs-${String(r)}`],c!=="wrap"&&n[`wrap-xs-${String(c)}`],...p]}})(({ownerState:e})=>({boxSizing:"border-box",...e.container&&{display:"flex",flexWrap:"wrap",width:"100%"},...e.item&&{margin:0},...e.zeroMinWidth&&{minWidth:0},...e.wrap!=="wrap"&&{flexWrap:e.wrap}}),q,H,J,_);function Y(e,n){if(!e||e<=0)return[];if(typeof e=="string"&&!Number.isNaN(Number(e))||typeof e=="number")return[`spacing-xs-${String(e)}`];const i=[];return n.forEach(t=>{const r=e[t];if(Number(r)>0){const s=`spacing-${t}-${String(r)}`;i.push(s)}}),i}const ee=e=>{const{classes:n,container:i,direction:t,item:r,spacing:s,wrap:a,zeroMinWidth:c,breakpoints:u}=e;let o=[];i&&(o=Y(s,u));const l=[];u.forEach(f=>{const d=e[f];d&&l.push(`grid-${f}-${String(d)}`)});const p={root:["root",i&&"container",r&&"item",c&&"zeroMinWidth",...o,t!=="row"&&`direction-xs-${String(t)}`,a!=="wrap"&&`wrap-xs-${String(a)}`,...l]};return O(p,U,n)},te=w.forwardRef(function(n,i){const t=D({props:n,name:"MuiGrid"}),{breakpoints:r}=L(),s=I(t),{className:a,columns:c,columnSpacing:u,component:o="div",container:l=!1,direction:p="row",item:f=!1,rowSpacing:d,spacing:h=0,wrap:B="wrap",zeroMinWidth:M=!1,...v}=s,z=d||h,V=u||h,j=w.useContext(C),b=l?c||12:j,k={},y={...v};r.keys.forEach(x=>{v[x]!=null&&(k[x]=v[x],delete y[x])});const G={...s,columns:b,container:l,direction:p,item:f,rowSpacing:z,columnSpacing:V,wrap:B,zeroMinWidth:M,spacing:h,...k,breakpoints:r.keys},E=ee(G);return W.jsx(C.Provider,{value:b,children:W.jsx(X,{ownerState:G,className:K(E.root,a),as:o,ref:i,...y})})});export{te as G};
